#include "entity.hh"
extern "C" {
#include <libgte.h>
}
// TODO
class Model : public Entity {
public:
    virtual int Kind() const override { return 0x9; }

public:
    void* m_Header; // is this correct in the original game?
    void* m_Data;
    VECTOR m_Vec;
};