#include "gameServer.h"

GameServer::GameServer(const int& gameServerPort, Client* clients)
{
	m_alive = true;
	m_serverPort = gameServerPort;
	m_futureObj = m_exitSignal.get_future();
	m_terminateThread = std::thread(&GameServer::WaitForTerminate, this);
	
	m_clients = clients;
	m_actionQueue = SharedQueue<Action*>();
	m_eventQueue = SharedQueue<Event*>();
	m_networkManager = new NetworkManager(std::move(m_futureObj), gameServerPort, m_actionQueue);
	m_actionManager = new ActionManager(clients, m_actionQueue, m_eventQueue);
	m_ecs = new ECS(m_networkManager, m_eventQueue, clients);

    //Main loop
    m_mainLoopThread = std::thread(&GameServer::Loop, this);
}

GameServer::GameServer()
{

}

GameServer::~GameServer()
{
	m_mainLoopThread.join();
	m_terminateThread.join();
	delete m_networkManager;
	delete m_actionManager;
	delete m_ecs;
	delete[] m_clients;
}

void GameServer::Loop()
{
	while(m_alive)
	{
		m_actionManager->Loop();
		if(!m_ecs->Loop()) Stop();
	}
}

void GameServer::Stop()
{
	if(m_futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
		m_exitSignal.set_value();
    m_alive = false;
}

void GameServer::WaitForTerminate()
{
    while(m_alive && m_futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout);
		std::this_thread::sleep_for(std::chrono::milliseconds(POST_GAME_DURATION));
}
