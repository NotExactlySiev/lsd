#pragma once
#include "entity.hh"
#include "subsystems.hh"

class Parent : Entity {
protected:
    static constexpr Rect2D c_Screen = { 0x140, 0xF0 };

    static bool s_CdInitialized;

    Rect2D m_Screen;
    int m_VramMode;
    bool m_Initialized;
    Subsystems* m_Subsystems;

public:
    Parent(int arg);
    virtual ~Parent();
    virtual void PreInit(const Rect2D* screen, int vramMode);
    virtual void Init(GPU* gpu, Pad* pad);
    virtual void DoNothing();
    virtual void Run();
    virtual void Func0() = 0;
    virtual void Func1() = 0;
    virtual int Func2() = 0;
    virtual void Func3() = 0;
    virtual bool Func4() = 0;
    virtual void Func5() = 0;
};