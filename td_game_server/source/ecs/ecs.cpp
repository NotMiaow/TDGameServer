#include "ecs.h"

ECS::ECS()
{
    //Fields
    m_alive = true;

    //Components
    m_ints = CheckpointList<int>();

    //Event handler
    m_eventHandler = EventHandler();
    m_eventHandler.Seed(m_ints);

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