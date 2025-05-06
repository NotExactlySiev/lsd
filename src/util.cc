#include "util.hh"

void Area2DToRECT(RECT* dst, Area2D* src) {
    dst->x = src->x;
    dst->y = src->y;
    dst->w = src->rect.w;
    dst->h = src->rect.h;
}

unsigned long SetBitfield(unsigned long& bitfield, uint position, int length, uint value) {
  position &= 32 - 1;
  uint mask = 1;
  for (int i = 0; i < length; i++)
      mask = mask << 1;

  mask = (mask - 1) << position;
  uint oldValue = (bitfield & mask) >> position;
  bitfield = (bitfield & ~mask) | (value << position);
  return oldValue;
}