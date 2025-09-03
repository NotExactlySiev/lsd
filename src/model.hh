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

    static int s_FirstModelLoaded;
    
public:
    Model(ulong* tmdData);

    /*  0 */ virtual void SetVector();
    /*  1 */ virtual void MapModelingData();
    /*  2 */ virtual void GetModelRawData();
    /*  3 */ virtual void Nothing();

    static void SetFirstModelLoaded();
    static int IsFirstModelLoaded();

    void* m_Header; // is this correct in the original game?
    void* m_Data;
    VECTOR m_Vec;
};