#pragma once
#include "file.hh"
#include "model.hh"

// TODO
class ModelSet : public File {
public:
    virtual int Kind() const override { return 0xD03; }

    struct Params {
        void* buffer;
        char* fileName;
    };

    ModelSet(Params* params);
    virtual ~ModelSet();

    virtual void ReadAll(char* filename) override;

    virtual void MapData();
    virtual void* GetModelData(int index);
    virtual Model* GetModel(int index);
    virtual void DoNothing();

private:
    Model** m_Models;
};