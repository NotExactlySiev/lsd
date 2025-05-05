#pragma once
#include "entity.hh"
extern "C" {
#include <libetc.h>
}

class Pad : Entity {
public:
    virtual int Kind() const override { return 0x2; }

    Pad(int mode, int id);
    virtual ~Pad();
    virtual void Init(int id);
    virtual void Read();
    virtual void ProcessAndSend();
    virtual void _PadNothing0();
    virtual void LoadMapping();
    virtual void _PadNothing1();
private:
    static int s_UserCount; // TODO: how initialize?
    static unsigned int s_Mapping[16];

    static constexpr unsigned int c_DefaultMapping[16] = {
        0x1000, 0x4000, 0x8000, 0x2000,
        0x0010, 0x0040, 0x0080, 0x0020,
        0x0200, 0x0400, 0x0100, 0x0008,
        0x0002, 0x0004, 0x0001, 0x0800,
    };

    short m_Id;
    unsigned int m_Raw;
    unsigned int m_NegEdge;
    unsigned int m_PosEdge;
    int unk;
};