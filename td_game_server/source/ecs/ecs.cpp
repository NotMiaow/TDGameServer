#include "ecs.h"

ECS::ECS(std::shared_future<void>&& serverFuture, NetworkManager* networkmanager, Queue<Event*>* eventQueue, Client* clients, std::atomic<bool>& serverAlive)
{
	m_serverFuture = serverFuture;
    m_serverAlive = &serverAlive;
    m_alive = true;

    //Components
    m_ints = CheckpointList<int>();

    //Event handler
    m_eventManager = EventManager();
    m_eventManager.Seed(m_ints);

    //Systems
    m_addSystem = AddSystem(m_ints);

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
        m_addSystem.ProcessData();
        
        //Kill server from within ecs
        //*m_serverAlive = false;
        //Example: After 3 loops
        //TODO : implement end game sequence after this loop
        //Post game duration can be modified in cst.h file
        if(cnt++ == 3) *m_serverAlive = false;
    }
}

void ECS::WaitForTerminate()
{
    while(m_serverFuture.wait_for(std::chrono::milliseconds(1000)) == std::future_status::timeout) { }
    m_alive = false;
}
