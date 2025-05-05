#pragma once
#include "entity.hh"

class Counter : Entity {
public:
    virtual int Kind() const override { return 0x5; }
    
    Counter();
    virtual ~Counter();
    virtual void RemoveListener(Entity* other) override;
    virtual void Broadcast(int) override;
    virtual void Init(int value);
    virtual void Tick();
    virtual int GetValue();
    virtual void Pause();
    virtual void Unpause();
    virtual bool IsPaused();
    virtual void Stop();

private:
    int m_Value;
    bool m_Paused;
    bool m_Stopped;
    List<Entity>::Node* m_WorkingList;    // Iterator
};