#include "list.hh"

// TODO: event enums

class Entity {
public:
    virtual int Kind() const { return 0; }

    Entity();
    virtual ~Entity();
    virtual void StartListening(Entity* other);
    virtual void StopListening(Entity* other);
    virtual void ClearListening();
    virtual void IterateListening(Entity*& ptr, List<Entity>::Node*& rest);

    virtual void AddListener(Entity* other);
    virtual void RemoveListener(Entity* other);
    virtual void ClearListeners();
    virtual void IterateListener(Entity*& ptr, List<Entity>::Node*& rest);
    
    virtual void Broadcast(int event);
    virtual void Nothing();
    virtual void ReceiveEvent(Entity* sender, int event);
    virtual void Unknown() = 0;

protected: 
    List<Entity> m_Listeners;
    List<Entity> m_Listening;
};