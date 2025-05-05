#include "object.hh"

struct Point {
    short x, y;

    int What() {
        // ...
        return ((x / y) * 4096) + ((x % y) * 4096) / y;
    }
};

Object::Object() : Entity() {
    m_Obj.coord2 = new GsCOORDINATE2;
    m_Obj.coord2->param = new GsCOORD2PARAM;
    unk4 = 0;
    m_Obj.tmd = nullptr;
    m_Super = nullptr;
    m_Obj.coord2->super = nullptr;
    Init();
}

Object::~Object() {
    // TODO 4
    // TODO 5
    // TODO 7
    delete m_Obj.coord2->param;
    delete m_Obj.coord2;
    Entity::~Entity();
}

void Object::ReceiveEvent(Entity* sender, int event) {
    Entity::ReceiveEvent(sender, event);
    switch (sender->Kind()) {
        // TODO
        case 2: break;
        case 4: break;
        case 5: break;
    }
}

void Object::Init() {
    unk5 = 0;
    m_Obj.attribute = 0;
    GsInitCoordinate2(nullptr, m_Obj.coord2);
    // TODO 1
    // TODO 2
    m_Obj.coord2->flg = 1;
}

//

//

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

//
//

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