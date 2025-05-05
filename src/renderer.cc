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
    Func20();
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

//
