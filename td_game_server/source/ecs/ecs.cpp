#include "ecs.h"

ECS::ECS(NetworkManager* networkmanager, SharedQueue<Event*>& eventQueue, Client* clients, std::shared_future<void>&& serverFuture, std::atomic<bool>& serverAlive)
{
    //Components
    m_motors = CheckpointList<MotorComponent>();
    m_transforms = CheckpointList<TransformComponent>();


    //Event handler
    m_eventManager = EventManager(m_motors, m_transforms);

    //Systems
    m_timeSystem = TimeSystem();
    m_movementSystem = MovementSystem(m_timeSystem, m_motors, m_transforms);

    //Server termination
	m_serverFuture = serverFuture;
    m_serverAlive = &serverAlive;
    m_alive = true;
    m_terminateThread = std::thread(&ECS::WaitForTerminate, this);
    m_mainLoopThread = std::thread(&ECS::Loop, this);
}

ECS::~ECS()
{
    m_terminateThread.join();
    m_mainLoopThread.join();
}

void ECS::Loop()
{
    int cnt = 1;
    while(m_alive)
    {
        m_timeSystem.Loop();
//        m_movementSystem.Loop();

        //Kill server from within ecs
        //*m_serverAlive = false;
        //Example: After 3 loops
        //Post game duration can be modified in cst.h
//        if(cnt++ == 3) *m_serverAlive = false;
    }
    //POST GAME LOOP HERE
}

void ECS::WaitForTerminate()
{
    while(m_serverFuture.wait_for(std::chrono::milliseconds(1000)) == std::future_status::timeout) { }
    m_alive = false;
}
