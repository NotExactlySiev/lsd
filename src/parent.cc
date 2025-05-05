
#include "parent.hh"
extern "C" {
#include <libcd.h>
}

extern int DAT_8008a8e0[];  // struct? config?

Parent::Parent(int arg) : Entity() {
    if (!s_CdInitialized) {
        CdInit();
        s_CdInitialized = true;
    }
    m_Initialized = false;
    FUN_80026cfc(arg);
    PreInit(&c_Screen, 0);
}

Parent::~Parent() {}

void Parent::PreInit(const Rect2D* screen, int vramMode) {
    m_Screen = *screen;
    m_VramMode = vramMode;
}

void Parent::Init(GPU* gpu, Pad* pad) {
    if (!m_Initialized) {
        GPU::SetGlobal(gpu);
        gpu->Init(&m_Screen, m_VramMode);
        FUN_80032368();
        GsInit3D();
        m_Subsystems = new Subsystems;
        m_Subsystems->gpu = gpu;
        m_Subsystems->pad = pad;
        m_Subsystems->counter = 0;
        m_Subsystems->unk0 = 0;
        m_Subsystems->unk1 = 0;
        m_Initialized = true;
    }
}

void Parent::DoNothing() {

}

void Parent::Run() {
    if (!m_Initialized)
        return;
    
    Func0();
again:
    Func1();
    while (true) {
        while (true) {
            int rc = Func2();
            if (rc == 0)
                goto again;
            if (rc == 1)
                break;
            if (rc == 2) {
                if (Func4())
                    Func5();
            }
        }
        Func3();
    }
}