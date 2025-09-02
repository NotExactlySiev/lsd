#include "renderer.hh"

Renderer::Renderer() : Entity() {
    m_Gpu = nullptr;
    m_Super = nullptr;
    m_Obj = new Object();
    // TODO
    // TODO
    Init();
}

Renderer::~Renderer() {
    Deallocate();
    Detatch();
    delete m_Obj;
    Func26(0);
    Entity::~Entity();
}

void Renderer::StartListening(Entity* other) {
    Entity::StartListening(other);
    switch (other->Kind() & 0xf) {
        case 1: {
            m_Gpu = static_cast<GPU*>(other);
            break;
        }

        case 4: {
            Object* obj = static_cast<Object*>(other);
            m_Super = obj;
            m_Refview.super = obj->m_Obj.coord2;
            break;
        }
    }
}

void Renderer::StopListening(Entity* other) {
    switch (other->Kind() & 0xf) {
        case 1: {
            m_Gpu = nullptr;
            break;
        }
        
        case 4: {
            m_Refview.super = nullptr;
            m_Super = nullptr;
            break;
        }
    }
    Entity::StopListening(other);
}

void Renderer::ClearListening() {
    m_Refview.super = nullptr;
    m_Super = nullptr;
    m_Gpu = nullptr;
    Entity::ClearListening();
}

void Renderer::ReceiveEvent(Entity* sender, int event) {
    Entity::ReceiveEvent(sender, event);
    switch (sender->Kind() & 0xf) {
        case 1: OnGpuEvent(static_cast<GPU*>(sender), event); break;
        case 5: On5Event(sender, event); break;
    }
}

void Renderer::Init() {
    m_Ev5Counter = 0;
    m_Allocated = false;
    m_Width = 256;
    m_Height = 240;
    m_OtPower = 13;
    m_PrimBucketCount = 2000;
    m_PrimBucketSize = 64;
    m_Projection = 256;
    m_NearClip = 10;
    m_FarClip = 16*4096;
    m_LightMode = 0;
    m_FogNear = 20000;
    m_FarColor = {};
    m_Color = {};
    //TODO
    m_Drawing = true;
}

void Renderer::SetDimensions(Rect2D* rect) {
    m_Width = rect->w;
    m_Height = rect->h;
}

void Renderer::SetOtPower(int val) {
    m_OtPower = val;
}

void Renderer::SetPrimBucketCount(int val) {
    if (!m_Allocated) {
        m_PrimBucketCount = val;
    }
}

void Renderer::SetPrimBucketSize(int val) {
if (!m_Allocated) {
        m_PrimBucketSize = val;
    }
}

void Renderer::SetProjection(int val) {
    m_Projection = val;
}

void Renderer::DoNothing0() {}
void Renderer::DoNothing1() {}

void Renderer::SetLightMode(int val) {
    m_LightMode = val;
}

void Renderer::SetColor(Color* color) {
    m_Color = *color;
}

void Renderer::SetFarColor(Color* color) {
    m_FarColor = *color;
}

void Renderer::SetFogNear(int val) {
    m_FogNear = val;
}

//
//

void Renderer::Detatch() {
    if (m_Super) {
        StopListening(m_Super);
    }
}

//



void Renderer::DoNothing2() {}
void Renderer::DoNothing3() {}

void Renderer::Allocate() {
    if (!m_Allocated) {
        // why this formula?
        size_t workSize = m_PrimBucketSize * m_PrimBucketCount;
        size_t otSize = (1 << m_OtPower) * sizeof(u_long);
        size_t bufferSize = sizeof(GsOT) + otSize + workSize;
        char* buffer = new char[2 * bufferSize];
        if (buffer == nullptr)
            return;
        
        m_OtHead[0] = (GsOT*) buffer;
        m_Ot[0] = buffer + sizeof(GsOT);
        m_WorkArea[0] = buffer + sizeof(GsOT) + otSize;

        m_OtHead[1] = (GsOT*) (buffer + bufferSize);
        m_Ot[1] = buffer + bufferSize + sizeof(GsOT);
        m_WorkArea[1] = buffer + bufferSize + sizeof(GsOT) + otSize;

        m_OtHead[0]->length = m_OtPower;
        m_OtHead[0]->org = (GsOT_TAG*) m_Ot[0];
        m_OtHead[1]->length = m_OtPower;
        m_OtHead[0]->org = (GsOT_TAG*) m_Ot[1];
        GsClearOt(0, 0, m_OtHead[0]);
        GsClearOt(0, 0, m_OtHead[1]);
        m_Allocated = true;
        m_BufferIndex = 0;
    }
}

void Renderer::Deallocate() {
    if (m_Allocated) {
        DrawSync(0);
        delete m_OtHead[0];
        m_Allocated = false;
    }
}

void Renderer::On5Event(Entity* sender, int event) {
    m_Ev5Counter++;
    if (event - 2U < 2) {
        Func23();
    }
}

void Renderer::OnGpuEvent(GPU* sender, int event) {
    if (event == 2) {
        DrawAndSwap();
    }
}

void Renderer::Func23() {
    if (m_Allocated) {
        if (m_Super->m_Super) {
            RenderObject(m_Super);
        }
        GsSetProjection(m_Projection);
        // TODO: where the fuck is GsSetNearClip???
        GsSetLightMode(m_LightMode);
        if (m_LightMode == 1 || m_LightMode == 3) {
            ::SetFarColor(m_FarColor.r, m_FarColor.g, m_FarColor.b);
            ::SetFogNear(m_FogNear, m_Projection);
        }
        GsSetRefView2(&m_Refview);
        m_Refview.super->flg = 0;
        m_DepthThing = (m_FarClip - m_NearClip) / (1 << m_OtPower) + 1;
        GsSetWorkBase((PACKET*) m_WorkArea[m_BufferIndex]);
        GsClearOt(0, 0, m_OtHead[m_BufferIndex]);
        RenderObject(m_Obj);
        if (m_Super) {
            RenderObject(GetRoot(m_Super));
        }
    }
}

void Renderer::DrawAndSwap() {
    if (!m_Allocated)
        return;
    
    m_BufferIndex = m_Gpu->GetActiveBuff();
    if (m_Drawing) {
        ResetGraph(1);
        m_Gpu->Swap();
        if (m_Unk9 && m_BufferIndex == 0) {
            m_Gpu->Swap();
        }
        GsSortClear(m_Color.r, m_Color.g, m_Color.b, m_OtHead[m_BufferIndex]);
        GsDrawOt(m_OtHead[m_BufferIndex]);
        if (m_Unk9 && m_BufferIndex == 0) {
            m_Gpu->Swap();
        }
    }
    m_BufferIndex = !m_BufferIndex;
}

//

void Renderer::SetDrawingEnable(bool enabled) {
    m_Drawing = enabled;
}

Object* Renderer::GetRoot(Object* obj) {
    while (obj->m_Super) {
        obj = obj->m_Super;
    }
    return obj;
}