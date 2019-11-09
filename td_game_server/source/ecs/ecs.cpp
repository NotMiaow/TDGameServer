#include "ecs.h"

ECS::ECS(bool& terminate, NetworkManager* networkmanager, Queue<Event*>* eventQueue, Client* clients)
{
    //Fields
    m_alive = true;

    //Components
    m_ints = CheckpointList<int>();

    //Event handler
    m_eventManager = EventManager();
    m_eventManager.Seed(m_ints);

    //Systems
    m_addSystem = AddSystem(m_ints);
}

void ECS::Loop()
{
    int cnt = 1;
    while(m_alive)
    {
        m_addSystem.ProcessData();
        if(cnt++ == 3) m_alive = false;
    }
}
