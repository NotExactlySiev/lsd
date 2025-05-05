#include "gpu.hh"

extern "C" {
#include <libetc.h>
#include <libgs.h>
}

GPU::GPU() : Entity() {
    PreInit();
}

void GPU::PreInit() {
    m_Running = 0;
    SetFrameStep(3);
    SetSync(true);
    m_Callback = nullptr;
}

void GPU::Init(Rect2D* screen, int vramMode) {
    GsInitGraph(screen->w, screen->h, 0, 1, vramMode);
    GsDefDispBuff(0, 0, 0, screen->h);
    m_Screen = *screen;
    m_VramMode = vramMode;
}

void GPU::Start() {
    if (!m_Running) {
        m_Running = 1;
        DoLoop();
    }
}

void GPU::Stop() {
    if (m_Running) {
        m_Running = 0;
    }
}

void GPU::Swap() {
    GsSwapDispBuff();
}

int GPU::GetActiveBuff() {
    return GsGetActiveBuff();
}

void GPU::Upload(Area2D* dst, void* src) {
    if (!m_Running || m_Sync) {
        RECT rect;
        Area2DToRECT(&rect, dst);
        LoadImage(&rect, static_cast<u_long*>(src));
        if (m_Sync) {
            DrawSync(0);
        }
    }
}

void GPU::Download(void* dst, Area2D* src) {
    if (!m_Running || m_Sync) {
        RECT rect;
        Area2DToRECT(&rect, src);
        StoreImage(&rect, static_cast<u_long*>(dst));
        if (m_Sync) {
            DrawSync(0);
        }
    }
}

int GPU::GetZero() {
    return 0;
}

void GPU::Move(Area2D* area, short x, short y) {
    RECT rect;
    Area2DToRECT(&rect, area);
    MoveImage(&rect, x, y);
}

void GPU::DoLoop() {
    while (m_Running) {
        VSync(m_FrameStep);
        if (m_Callback) {
            m_Callback();
        }
        Broadcast(2);
    }
}

//

void GPU::SetFrameStep(int frames) {
    m_FrameStep = frames;
}

int GPU::GetFrameStep() {
    return m_FrameStep;
}

void GPU::Fill(CVECTOR* color, Area2D* area) {
    RECT rect;
    if (area) {
        Area2DToRECT(&rect, area);
        ClearImage(&rect, color->r, color->g, color->b);
    } else {
        // do whole screen
        Area2D screenArea;
        GetScreenArea(&screenArea);
        Fill(color, &screenArea);
    }
}

void GPU::GetScreenArea(Area2D* area) {
    if (area) {
        area->x = 0;
        area->y = 0;
        area->rect = m_Screen;
    }
}

void GPU::SetSync(bool enabled) {
    m_Sync = enabled;
}

void GPU::SetCallback(void (*callback)()) {
    m_Callback = callback;
}


void GPU::SetGPU(GPU* gpu) {
    s_Global = gpu;
}

GPU* GPU::GetGlobal() {
    return s_Global;
}