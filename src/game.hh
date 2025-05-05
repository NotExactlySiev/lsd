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
public:
    virtual int Kind() const override { return 0x1F60; }

    Game(GameConfig* config);
    virtual void PreInit();
    virtual void Init(GPU* gpu, Pad* pad);
    virtual void Func0();
    virtual void Func1();
    virtual int Func2();
    virtual void Func3();
    virtual bool Func4();
    virtual void Func5();

private:
    GameConfig* m_Config;
};
