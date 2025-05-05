#pragma once
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
    /*  0 */ virtual void Init();
    /*  1 */ virtual void Func1();
    /*  2 */ virtual void Func2();
    /*  3 */ virtual void Attach(Object* super, VECTOR* offset);
    /*  4 */ virtual void Detach();
    /*  5 */ virtual void DetachChildren();
    /*  6 */ virtual void IterateChildren(Object*& ptr, List<Entity>::Node*& rest);
    /*  7 */ virtual void Func7();   // no op
    /*  8 */ // flag stuff
    /*  9 */ // flag stuff
    /* 10 */ // flag stuff
    /* 11 */ // flag stuff
    /* 12 */ // flag stuff
    /* 13 */ // flag stuff
    /* 14 */ // flag stuff
    /* 15 */ // flag stuff
    /* 16 */ // flag stuff
    /* 17 */ virtual void MakeRotationMatrix(MATRIX* out, bool flip);
    /* 18 */
    /* 19 */
    /* 20 */
    /* 21 */ // handler
    /* 22 */ // handler
    /* 23 */ // handler
    /* 24 */
    /* 25 */
    /* 26 */
    /* 27 */
    /* 28 */
    /* 29 */

public:
    Object* m_Super;
    GsDOBJ2 m_Obj;
    int unk4;
    int unk5;
};
