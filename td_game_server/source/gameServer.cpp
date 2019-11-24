#include "gameServer.h"

GameServer::GameServer(const int& gameServerPort, Client* clients)
{
	//Terminate
	m_alive = true;
	m_serverPort = gameServerPort;
	m_futureObj = m_exitSignal.get_future();
	m_terminateThread = std::thread(&GameServer::WaitForTerminate, this);
	
	//Networking
	m_clients = clients;
	m_networkManager.Init(std::move(m_futureObj), gameServerPort, m_actionQueue);
	m_actionManager.Init(clients, m_actionQueue, m_eventQueue);
	
	InitialiseComponentDataStructure();
	
	//ECS
	m_eventManager.Init(m_clients, m_networkManager, m_eventQueue, m_players, m_banks, m_offenses, m_motors, m_transforms);	
	m_ecs.Init(m_eventQueue, m_players, m_banks, m_offenses, m_motors, m_transforms);


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
	delete[] m_clients;
}

void GameServer::Loop()
{
	while(m_alive)
	{
		m_actionManager.Loop();
		m_eventManager.Loop();
		if(!m_ecs.Loop()) Stop();
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

void GameServer::InitialiseComponentDataStructure()
{
	//PlayerComponent
	{
		TabNode<PlayerComponent>* tabIt = m_players.GetTabHead();
		for(int i = 0; i < MAX_CLIENTS; i++)
			tabIt = m_players.InsertTab(tabIt);
	}
	//BankComponent
	{
		TabNode<BankComponent>* tabIt = m_banks.GetTabHead();
		for(int i = 0; i < MAX_CLIENTS; i++)
			tabIt = m_banks.InsertTab(tabIt);
	}
	//OffenseComponent
	{
		TabNode<OffenseComponent>* tabIt = m_offenses.GetTabHead();
		CheckpointNode<OffenseComponent>* checkpointIt;
		for(int i = 0; i < MAX_CLIENTS; i++)
		{
			tabIt = m_offenses.InsertTab(tabIt);
			checkpointIt = tabIt->checkpointNode;
			for(int i = 1; i < 2; i++)
				checkpointIt = m_offenses.InsertCheckpoint(checkpointIt);
		}
	}
	//Motors
	{
		TabNode<MotorComponent>* tabIt = m_motors.GetTabHead();
		for(int i = 0; i< MAX_CLIENTS; i++)
			tabIt = m_motors.InsertTab(tabIt);
	}
	//TransformComponent
	{
		TabNode<TransformComponent>* tabIt = m_transforms.GetTabHead();
		CheckpointNode<TransformComponent>* checkpointIt;
		for(int i = 0; i < MAX_CLIENTS; i++)
		{
			tabIt = m_transforms.InsertTab(tabIt);
			checkpointIt = tabIt->checkpointNode;
			for(int i = 1; i < 2; i++)
				checkpointIt = m_transforms.InsertCheckpoint(checkpointIt);
		}
	}
}
