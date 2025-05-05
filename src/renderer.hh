#pragma once
#include "entity.hh"
#include "gpu.hh"
#include "object.hh"
#include "util.hh"
extern "C" {
#include "libgs.h"
}

class Renderer : Entity {
public:
    virtual int Kind() const { return 0x7; }

    Renderer();
    virtual ~Renderer();
    virtual void StartListening(Entity* other) override;
    virtual void StopListening(Entity* other) override;
    virtual void ClearListening() override;
    virtual void ReceiveEvent(Entity* sender, int event) override;

    /*  0 */ virtual void Init();
    /*  1 */ virtual void SetDimensions(Rect2D* rect);
    /*  2 */ virtual void SetOtPower(int val);
    /*  3 */ virtual void SetPrimBucketCount(int val);
    /*  4 */ virtual void SetPrimBucketSize(int val);
    /*  5 */ virtual void SetProjection(int val);
    /*  6 */ virtual void DoNothing0();
    /*  7 */ virtual void DoNothing1();
    /*  8 */ virtual void SetLightMode(int val);
    /*  9 */ virtual void SetColor(Color* color);
    /* 10 */ virtual void SetFarColor(Color* color);
    /* 11 */ virtual void SetFogNear(int val);
    /* 12 */ virtual void SetRefview();
    /* 13 */ virtual void Detatch();
    /* 14 */ virtual void SetRefviewPos();
    /* 15 */ virtual void SetRefviewRot();
    /* 16 */ virtual void SetRefviewZ();
    /* 17 */ virtual void DoNothing2();
    /* 18 */ virtual void DoNothing3();
    /* 19 */ virtual void AllocateMemory();
    /* 20 */ virtual void Func20();
    /* 21 */ virtual void On5Event(Entity* sender, int event);
    /* 22 */ virtual void OnGpuEvent(GPU* sender, int event);
    /* 23 */ virtual void Func23();
    /* 24 */ virtual void Func24DrawObject();
    /* 25 */ virtual void DrawAndSwap();
    /* 26 */ virtual void Func26(int);
    /* 27 */ virtual void Func27();
    /* 28 */ virtual void Func28();
    /* 29 */ virtual void SetDrawingEnable();


private:
    GPU* m_Gpu;
    Object* m_Super;
    GsRVIEW2 m_Refview;
    int m_Width;
    int m_Height;
    int m_OtPower;
    int m_Projection;
    int m_PrimBucketCount;
    int m_PrimBucketSize;
    int m_NearClip;
    int m_FarClip;
    int m_LightMode;
    // TODO: what's the alignment for these two like?
    Color m_Color;
    Color m_FarColor;
    int m_FogNear;
    int m_Unk0;
    int m_Unk1;
    int m_Unk2;
    int m_Allocated;
    int m_BufferIndex;
    GsOT* m_Ots[2];
    void* m_WorkAreas[2][2];
    int m_Ev5Counter;
    int m_Unk3;
    uint m_DepthThing;
    int m_Unk4;
    int m_Unk5;
    int m_Unk6;
    int m_Unk7;
    Object* m_Obj;
    //
    //
    bool m_Drawing;
};