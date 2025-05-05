#include "object.hh"
#include "util.hh"

struct Point {
    short x, y;

    int What() {
        // huh.
        return ((x / y) * 4096) + ((x % y) * 4096) / y;
    }
};

Object::Object() : Entity() {
    m_Obj.coord2 = new GsCOORDINATE2;
    m_Obj.coord2->param = new GsCOORD2PARAM;
    m_Model = nullptr;
    m_Obj.tmd = nullptr;
    m_Super = nullptr;
    m_Obj.coord2->super = nullptr;
    Init();
}

Object::~Object() {
    Detach();
    DetachChildren();
    DoNothing0(0);
    delete m_Obj.coord2->param;
    delete m_Obj.coord2;
    Entity::~Entity();
}

void Object::StartListening(Entity* other) {
    Entity::StartListening(other);
    if (other->Kind() & 0xf == 9) {
        SetModel(static_cast<Model*>(other));
    }
}

void Object::StopListening(Entity* other) {
    if (other->Kind() & 0xf == 9) {
        ResetModel();
    }
    Entity::StopListening(other);
}

void Object::ClearListening() {
    ResetModel();
    Entity::ClearListening();
}

void Object::ReceiveEvent(Entity* sender, int event) {
    Entity::ReceiveEvent(sender, event);
    switch (sender->Kind()) {
        // TODO
        case 2: On2Event(sender, event); break;
        case 4: OnObjectEvent(static_cast<Object*>(sender), event); break;
        case 5: On5Event(sender, event); break;
    }
}

static Point rotatePoints[3] = {{0,1}, {0,1}, {0,1}};
static Point scalePoints[3] = {{1,1}, {1,1}, {1,1}};

void Object::Init() {
    unk5 = 0;
    m_Obj.attribute = 0;
    GsInitCoordinate2(nullptr, m_Obj.coord2);
    Func1(1, rotatePoints);
    Func2(1, scalePoints);
    m_Obj.coord2->flg = 1;
}

void Object::Func1(int set, Point* points) {
    int s[3] = { 
        points[0].What() / 360,
        points[1].What() / 360,
        points[2].What() / 360,
    };
    SVECTOR& rotate = m_Obj.coord2->param->rotate;
    if (set) {
        rotate.vx = s[0];
        rotate.vy = s[1];
        rotate.vz = s[2];
    } else {
        short* p = (short *) &rotate;
        for (int i = 0; i < 3; i++) {
            // what is even happening
            int tmp = p[i] + s[i];
            p[i] = tmp + tmp/4096;
        }
    }
    m_Obj.coord2->flg = 0;
}

void Object::Func2(int set, Point* points) {
    short s0 = points[0].What();
    short s1 = points[1].What();
    short s2 = points[2].What();
    VECTOR& scale = m_Obj.coord2->param->scale;
    if (set) {
        scale.vx = s0;
        scale.vy = s1;
        scale.vz = s2;
    } else {
        scale.vx += s0;
        scale.vy += s1;
        scale.vz += s2;
    }
    m_Obj.coord2->flg = 0;
}

void Object::Attach(Object* super, VECTOR* offset) {
    if (m_Super)
        return;
    
    m_Super = super;
    m_Obj.coord2->super = super->m_Obj.coord2->super;
    super->StartListening(this);
    if (offset) {
        m_Obj.coord2->coord.t[0] = offset->vx;
        m_Obj.coord2->coord.t[1] = offset->vy;
        m_Obj.coord2->coord.t[2] = offset->vz;
    } else {
        m_Obj.coord2->coord.t[0] = 0;
        m_Obj.coord2->coord.t[1] = 0;
        m_Obj.coord2->coord.t[2] = 0;
    }
    m_Obj.coord2->flg = 0;
}

void Object::Detach() {
    if (m_Super) {
        m_Super->StopListening(this);
        m_Obj.coord2->super = nullptr;
        m_Super = nullptr;
    }
}

void Object::DetachChildren() {
    Object* child = nullptr;
    List<Entity>::Node* rest;   // TODO: should this be called context?
    do {
        IterateChildren(child, rest);
        if (child) {
            child->Detach();
        }
    } while (rest);
}

void Object::IterateChildren(Object*& ptr, List<Entity>::Node*& rest) {
    // keep iterating until a child is found, then return
    Entity* p = ptr;
    while (true) {
        if (ptr == nullptr) {
            rest = m_Listening.m_Head;
        }
        List<Entity>::Iterate(p, rest);
        // TODO: is this basically just a dynamic cast?
        if (p && p->Kind() == 4 && static_cast<Object*>(p)->m_Super == this) {
                ptr = static_cast<Object*>(p);
                break;
        }
        if (rest == nullptr) {
            ptr = nullptr;
            return;
        }
    }
}

void Object::DoNothing0(int) {}

int Object::SetDisplay(int val) {
    return !SetBitfield(m_Obj.attribute, 31, 1, !val);
}

int Object::SetSemiTrans(int val) {
    return SetBitfield(m_Obj.attribute, 30, 1, (bool) val);
}

int Object::SetSemiTransMode(int val) {
    return SetBitfield(m_Obj.attribute, 28, 2, val);
}

int Object::SetLightSourceCalc(int val) {
    return SetBitfield(m_Obj.attribute, 6, 1, !val);
}

int Object::SetLightingMode(int val) {
    return SetBitfield(m_Obj.attribute, 3, 3, val);
}

int Object::SetMaterialAttenuation(int val) {
    return SetBitfield(m_Obj.attribute, 0, 3, val);
}

int Object::SetNearClipping(int val) {
    return !SetBitfield(m_Obj.attribute, 7, 1, !val);
}

int Object::SetSubdivision(int val) {
    return SetBitfield(m_Obj.attribute, 9, 3, val);
}

int Object::SetBackClipping(int val) {
    return !SetBitfield(m_Obj.attribute, 8, 1, !val);
}

void Object::MakeRotationMatrix(MATRIX* out, bool flip) {
    SVECTOR rotation;
    SVECTOR& param = m_Obj.coord2->param->rotate;
    if (flip) {
        rotation.vx = -param.vx;
        rotation.vy = -param.vy;
        rotation.vz = -param.vz;
    } else {
        rotation = param;
    }
    RotMatrix(&rotation, out);
}

//

void Object::On2Event(Entity* sender, int event) {}

void Object::On5Event(Entity* sender, int event) {}

void Object::OnObjectEvent(Object* sender, int event) {
    if (event > 1) {
        if (event < 4) {
            CheckCollision(sender);
        } else if (event == 4) {
            m_Other = sender;
        }
    }
}

//
//

void Object::DoNothing1() {}

//


void Object::SetModel(Model* model) {
    m_Model = model;
    m_Obj.tmd = (unsigned long*) model->m_Data;
    GsLinkObject4((unsigned long) model->m_Header + 3, &m_Obj, 0);
}

void Object::ResetModel() {    
    m_Obj.tmd = nullptr;
    m_Model = nullptr;
}

//