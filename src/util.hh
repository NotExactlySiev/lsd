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
void Area2DToRECT(RECT* dst, Area2D* src) {
    dst->x = src->x;
    dst->y = src->y;
    dst->w = src->rect.w;
    dst->h = src->rect.h;
}

struct Color {
    u_char r, g, b;
};

// this might be the funniest thing I've ever seen
static unsigned long SetBitfield(unsigned long& bitfield, uint position, uint length, uint value) {
  position &= 32 - 1;
  uint mask = 1;
  for (int i = 0; i < length; i++)
      mask = mask << 1;

  mask = (mask - 1) << position;
  uint oldValue = (bitfield & mask) >> position;
  bitfield = (bitfield & ~mask) | (value << position);
  return oldValue;
}