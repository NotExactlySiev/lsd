#pragma once
extern "C" {
#include <libgpu.h>
}
// This struct is sometimes read from as if x and y are u16. why?
struct Rect2D {
    unsigned int w, h;
};

struct Area2D {
    short x, y;
    Rect2D rect;
};

// TODO: maybe this originally returned RECT as value?
void Area2DToRECT(RECT* dst, Area2D* src);
unsigned long SetBitfield(unsigned long& bitfield, uint position, uint length, uint value);

struct Color {
    u_char r, g, b;
};