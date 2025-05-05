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