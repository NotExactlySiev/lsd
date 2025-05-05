#pragma once
#include "entity.hh"
#include "model.hh"
extern "C" {
#include <libgs.h>
#include <libgte.h>
}

class Object : public Entity {
public:
    virtual int Kind() const override { return 0x4; }

    Object();
    virtual void StartListening(Entity* other) override;
    virtual void StopListening(Entity* other) override;
    virtual void ClearListening() override;
    virtual void ReceiveEvent(Entity* sender, int event) override;

    virtual ~Object();
    /*  0 */ virtual void Init();
    /*  1 */ virtual void Func1(int set, Point* points);
    /*  2 */ virtual void Func2(int set, Point* points);
    /*  3 */ virtual void Attach(Object* super, VECTOR* offset);
    /*  4 */ virtual void Detach();
    /*  5 */ virtual void DetachChildren();
    /*  6 */ virtual void IterateChildren(Object*& ptr, List<Entity>::Node*& rest);
    /*  7 */ virtual void DoNothing0(int);   // no op
    /*  8 */ virtual int SetDisplay(int val);
    /*  9 */ virtual int SetSemiTrans(int val);
    /* 10 */ virtual int SetSemiTransMode(int val);
    /* 11 */ virtual int SetLightSourceCalc(int val);
    /* 12 */ virtual int SetLightingMode(int val);
    /* 13 */ virtual int SetMaterialAttenuation(int val);
    /* 14 */ virtual int SetNearClipping(int val);
    /* 15 */ virtual int SetSubdivision(int val);
    /* 16 */ virtual int SetBackClipping(int val);
    /* 17 */ virtual void MakeRotationMatrix(MATRIX* out, bool flip);
    /* 18 */
    /* 19 */
    /* 20 */
    /* 21 */ virtual void On2Event(Entity* sender, int event);
    /* 22 */ virtual void On5Event(Entity* sender, int event);
    /* 23 */ virtual void OnObjectEvent(Object* sender, int event);
    /* 24 */ virtual void CheckCollision(Object* other);  // I think
    /* 25 */
    /* 26 */
    /* 27 */
    /* 28 */ virtual void DoNothing1();
    /* 29 */

    void SetModel(Model* model);
    void ResetModel();

public:
    Object* m_Super;
    GsDOBJ2 m_Obj;
    Model* m_Model;
    int unk5;
    Object* m_Other;    // collider?
    //...
};
