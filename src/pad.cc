
#include "pad.hh"

void PadInit(int mode);
void PadStop();
unsigned int PadRead(int);

Pad::Pad(int mode, int id) : Entity() {
    if (s_UserCount++ == 0) {
        PadInit(mode);
    }
    Init(id);
}

Pad::~Pad() {
    if (--s_UserCount == 0) {
        PadStop();
    }
    Entity::~Entity();
}

void Pad::Init(int id) {
    m_Id = id != 0;
    m_Raw = 0;
    m_NegEdge = 0;
    m_PosEdge = 0;
    LoadMapping();
}

void Pad::Read() {
    unsigned int now = PadRead(m_Id);
    unsigned int old = m_Raw;
    m_Raw = now;
    unsigned int edge = now ^ old;
    m_NegEdge = edge & old;
    m_PosEdge = edge & now;
}

void Pad::ProcessAndSend() {
    int events[16];
    int n = 0;
    unsigned int raw = m_Raw;
    unsigned int neg = m_NegEdge;
    unsigned int pos = m_PosEdge;
    if (raw == 0 && neg == 0 && pos == 0)
        return;
    
    for (int i = 0; i < 16; i++) {
        unsigned int button = s_Mapping[i];
        int event = -1;
        if (neg & button) {
            event = 0x22;
        } else if (pos & button) {
            event = 0x12;
        } else if (raw & button) {
            event = 0x02;
        }
        if (event > -1) {
            events[n++] = event;
        }
    }

    while (n--) {
        Broadcast(events[n]);
    }
}

void Pad::_PadNothing0() {};

void Pad::LoadMapping() {
    unsigned int buffer[16]; // WHYYY
    __builtin_memcpy(buffer, c_DefaultMapping, sizeof(unsigned int[16]));
    __builtin_memcpy(s_Mapping, buffer, sizeof(unsigned int[16]));
}

void Pad::_PadNothing1() {};
