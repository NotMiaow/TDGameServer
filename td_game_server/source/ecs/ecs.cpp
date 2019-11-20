#include "ecs.h"

ECS::ECS(NetworkManager* networkManager, SharedQueue<Event*>& eventQueue, Client* clients)
{
    //Components
    m_players = CheckpointList<PlayerComponent>();
    m_resources = CheckpointList<ResourceComponent>();
    m_motors = CheckpointList<MotorComponent>();
    m_transforms = CheckpointList<TransformComponent>();

    //Event manager
    m_eventManager = EventManager(networkManager, clients, eventQueue, m_players, m_resources, m_motors, m_transforms);

    //Systems
    m_timeSystem = TimeSystem();
    m_movementSystem = MovementSystem(m_timeSystem, m_motors, m_transforms);
}

ECS::~ECS()
{
}

bool ECS::Loop()
{
    m_timeSystem.Loop();
//        m_movementSystem.Loop();
    m_eventManager.Loop();
    
    
    //Kill server from within ecs
//    return false;
    return true;
}

