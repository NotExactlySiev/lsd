#include "entity.hh"

extern "C" {
#include <libgs.h>
#include <libgte.h>
}

class Object : Entity {
public:
    virtual int Kind() const override { return 4; }

    // loads of functions
    Object(int);  // arg?
    // TODO 6 overrides
    virtual void ReceiveEvent(Entity* sender, int event) override;

    virtual ~Object();
    virtual void Init();
    virtual void Func1();
    virtual void Func2();
    virtual void Attach(Object* super, VECTOR* offset);
    virtual void Detach();
    virtual void DetachChildren();
    virtual void IterateChildren(Object*& ptr, List<Entity>::Node*& rest);
    virtual void Func7();   // no op
    // flag stuff
    // flag stuff
    // flag stuff
    // flag stuff
    // flag stuff
    // flag stuff
    // flag stuff
    // flag stuff
    // flag stuff
    virtual void RotateMatrix();


public:
    Object* m_Super;
    GsDOBJ2 m_Obj;
    int unk4;
    int unk5;
};
