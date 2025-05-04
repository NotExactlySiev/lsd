
#include "entity.hh"

Entity::Entity() {}

Entity::~Entity() {
    Broadcast(1);
    ClearListening();
    ClearListeners();
}

void Entity::StartListening(Entity* other) {
    if (m_Listening.Add(other)) {
        other->AddListener(this);
    }
}

void Entity::StopListening(Entity* other) {
    m_Listening.Remove(other);
    other->RemoveListener(this);
}

void Entity::ClearListening() {
    Entity* entity;
    List<Entity>::Node* iter;
    while (List<Entity>::Iterate(entity, iter), entity != nullptr) {
        StopListening(this);
    }
}

void Entity::IterateListening(Entity*& ptr, List<Entity>::Node*& rest) {
    if (ptr == nullptr) {
        rest = m_Listening.m_Head;
    }
    List<Entity>::Iterate(ptr, rest);
}

void Entity::AddListener(Entity* other) {
    m_Listeners.Add(other);
}

void Entity::RemoveListener(Entity* other) {
    m_Listeners.Remove(other);
}

void Entity::ClearListeners() {
    m_Listeners.Clear();
}

void Entity::IterateListener(Entity*& ptr, List<Entity>::Node*& rest) {
    if (ptr == nullptr) {
        rest = m_Listeners.m_Head;
    }
    List<Entity>::Iterate(ptr, rest);
}

void Entity::Nothing() {}

void Entity::ReceiveEvent(Entity* sender, int event) {
    if (event == 1) {
        // sender died
        StopListening(sender);
    }
}
