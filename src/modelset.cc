#include "modelset.hh"
extern "C" {
#include <libgs.h>
}

// why does the original have a return value here oh no
ModelSet::ModelSet(Params* params) : File() {
    if (params) {
        if (params->buffer) {
            m_Buffer = params->buffer;
            m_Size = 0;
            SetFlag();  // why does this have a return value here wtf
        } else {
            Func11(params->fileName);   // help me
        }
    }
}

ModelSet::~ModelSet() {
    for (Model** p = m_Models; *p; p++) {
        delete *p;
    }
    delete m_Models;
    File::~File();
}

// ReadAll

void ModelSet::MapData() {
    GsMapModelingData(((unsigned long*) m_Buffer) + 1);
}

void* ModelSet::GetModelData(int index) {
    return ((char*) m_Buffer) + 12 + 28 * index;
}

Model* ModelSet::GetModel(int index) {
    return m_Models[index];
}

void ModelSet::DoNothing() {}