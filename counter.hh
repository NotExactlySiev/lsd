#include "entity.hh"

class Counter : Entity {
    int m_Value;
    bool m_Paused;
    bool m_Stopped;
    List<Entity>::Node* m_WorkingList;    // Iterator
public:
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
};