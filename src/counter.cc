#include "counter.hh"

Counter::Counter() : Entity() {
    Init(0);
}

Counter::~Counter() {
}

// we override these two functions to stop infinite loops
void Counter::RemoveListener(Entity* other) {
    if (m_WorkingList && m_WorkingList->data == other) {
        m_WorkingList = m_WorkingList->next;
    }
    Entity::RemoveListener(other);
}

void Counter::Broadcast(int event) {
    m_WorkingList = m_Listeners.m_Head;
    Entity *listener;
    while (List<Entity>::Iterate(listener, m_WorkingList), listener) {
        listener->ReceiveEvent(this, event);
    }
    m_WorkingList = nullptr;
}

void Counter::Init(int value) {
    m_Value = value;
    m_Stopped = false;
    m_Paused = false;
    m_WorkingList = nullptr;
}

void Counter::Tick() {
    int event;
    if (m_Stopped)
        event = 4;
    else if (m_Paused)
        event = 3;
    else {
        event = 2;
        m_Value++;
    }
    Broadcast(event);
}

int Counter::GetValue() {
    return m_Value;
}

void Counter::Pause() {
    m_Paused = true;
}

void Counter::Unpause() {
    m_Paused = false;
}

bool Counter::IsPaused() {
    return m_Paused;
}

void Counter::Stop() {
    m_Stopped = true;
}
