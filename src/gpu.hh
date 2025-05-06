#pragma once
#include "entity.hh"
#include "util.hh"
extern "C" {
#include <libgte.h>
}

class GPU : public Entity {
public:
    virtual int Kind() const { return 0x1; }

    GPU();
    /*  0 */ virtual void PreInit();
    /*  1 */ virtual void Init(Rect2D* screen, int vramMode);
    /*  2 */ virtual void Start();
    /*  3 */ virtual void Stop();
    /*  4 */ virtual void Swap();
    /*  5 */ virtual int GetActiveBuff();
    /*  6 */ virtual void Upload(Area2D* dst, void* src);
    /*  7 */ virtual void Download(void* dst, Area2D* src);
    /*  8 */ virtual int GetZero();
    /*  9 */ virtual void Move(Area2D* area, short x, short y);
    /* 10 */ virtual void DoLoop();
    /* 11 */ virtual void Unknown0();   // tick
    /* 12 */ virtual void SetFrameStep(int step);
    /* 13 */ virtual int GetFrameStep();
    /* 14 */ virtual void Fill(CVECTOR* color, Area2D* area);
    /* 15 */ virtual void GetScreenArea(Area2D* area);
    /* 16 */ virtual void SetSync(bool enabled);
    /* 17 */ virtual void SetCallback(void (*callback)());

    static void SetGPU(GPU* gpu);
    static GPU* GetGlobal();

private:
    static GPU* s_Global;

    int unk0;
    int m_Running;
    Rect2D m_Screen;
    int m_VramMode;
    int m_FrameStep;
    int unk1;
    int unk2;
    int m_Sync;
    void (*m_Callback)();
};