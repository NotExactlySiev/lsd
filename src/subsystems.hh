#include "gpu.hh"
#include "pad.hh"
#include "counter.hh"

struct Subsystems {
    GPU* gpu;
    Pad* pad;
    Counter* counter;
    void* unk0;
    void* unk1;
};