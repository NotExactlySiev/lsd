#include "renderer.hh"

void Renderer::RenderObject(Object* obj) {
    
    if ((obj->Kind() & 0xff == 0x24) && (obj->m_Obj.attribute < 0))
        return;

    bool flag = false;
    MATRIX mat;
    GsCOORDINATE2* co = obj->m_Obj.coord2;
    if (co->flg == 0) {
        flag = true;
        RotMatrix(&co->param->rotate, &mat);
        for (int i = 0; i < 3; i++) {
            mat.m[i][0] = (mat.m[i][0] * co->param->scale.vx) >> 12;
            mat.m[i][1] = (mat.m[i][1] * co->param->scale.vy) >> 12;
            mat.m[i][2] = (mat.m[i][2] * co->param->scale.vz) >> 12;
        }
    }

    List<Entity>::Node* ctx;
    Object* child;
    do {
        obj->IterateChildren(child, ctx);   // inlined
        if (child) {
            if (flag) {
                child->m_Obj.coord2->flg = 0;
            }
            Func24DrawObject(child);
        }
    } while (ctx);

    switch (obj->Kind() & 0xFF) {
        case 0x54: {
            //
            break;
        }

        case 0x64: {
            //
            break;
        }

        case 0x44: {
            //
            break;
        }

        default: {
            if (obj->Kind() & 0xFFF == 0x144) {
                //
            } else {
                //
            }
            //
            break;
        }
    }
    //
    // TODO
}