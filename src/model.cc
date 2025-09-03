#include "model.hh"

Model::Model(ulong* tmdData) : Entity() {
    m_Data = tmdData;
    m_Header = tmdData - 3;
    //set_mystery_done();
    //
}

void Model::FindBoundingBox(Box& box) {
    SVECTOR* verts = *static_cast<SVECTOR**>(m_Data);
    uint n = *static_cast<uint*>(m_Data + 4);

    box.p0 = verts[0];
    box.p1 = box.p0;
    for (int i = 0; i < n; i++) {
        SVECTOR v = verts[i+1];
        if (box.p0.x > v.vx)
            box.p0.x = v.vx;
        if (box.p0.y > v.vy)
            box.p0.y = v.vy;
        if (box.p0.z > v.vz)
            box.p0.z = v.vz;

        if (box.p1.x < v.vx)
            box.p1.x = v.vx;
        if (box.p1.y < v.vy)
            box.p1.y = v.vy;
        if (box.p1.z < v.vz)
            box.p1.z = v.vz;

    }
}

void Model::UnwrapBoundingBox(ColBox& col) {
    Box box;
    FindBoundingBox(box);
    col.pts[0].x = box.p0.x;
    col.pts[0].y = box.p0.y;
    col.pts[0].z = box.p0.z;

    col.pts[1].x = box.p0.x;
    col.pts[1].y = box.p1.y;
    col.pts[1].z = box.p0.z;

    col.pts[2].x = box.p1.x;
    col.pts[2].y = box.p1.y;
    col.pts[2].z = box.p0.z;

    col.pts[3].x = box.p1.x;
    col.pts[3].y = box.p0.y;
    col.pts[3].z = box.p0.z;

    col.pts[4].x = box.p0.x;
    col.pts[4].y = box.p0.y;
    col.pts[4].z = box.p1.z;

    col.pts[5].x = box.p0.x;
    col.pts[5].y = box.p1.y;
    col.pts[5].z = box.p1.z;

    col.pts[6].x = box.p1.x;
    col.pts[6].y = box.p1.y;
    col.pts[6].z = box.p1.z;

    col.pts[7].x = box.p1.x;
    col.pts[7].y = box.p0.y;
    col.pts[7].z = box.p1.z;
    col.n = 1;
}