#pragma once
#include "entity.hh"
extern "C" {
#include <libgte.h>
}

struct Point3D {
    short x, y, z;

    void operator=(SVECTOR& vec) {
        x = vec.vx;
        y = vec.vy;
        z = vec.vz;
    }

    void operator+=(Point3D& other) {
        x += other.x;
        x += other.y;
        x += other.z;
    }
};

// AABB
struct Box {
    Point3D p0, p1;
};

struct ColBox {
    uint n;
    Point3D pts[][8];
};

// TODO
class Model : public Entity {
public:
    virtual int Kind() const override { return 0x9; }
    //

    void FindBoundingBox(Box& box);
    void UnwrapBoundingBox(ColBox& col);


public:
    void* m_Header; // is this correct in the original game?
    void* m_Data;
    VECTOR m_Vec;
};