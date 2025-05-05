#pragma once
#include "parent.hh"

struct GameConfig {
    int code;
    int unk1;
    int unk2;
    int unk3;
    int unk4;
    int unk5;
};

class Game : Parent {
    GameConfig* m_Config;
public:
    Game(GameConfig* config);
    virtual void PreInit();
    virtual void Init(GPU* gpu, Pad* pad);
    virtual void Func0();
    virtual void Func1();
    virtual int Func2();
    virtual void Func3();
    virtual bool Func4();
    virtual void Func5();
};
