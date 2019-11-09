#include "gameServer.h"

GameServer::GameServer(const int& gameServerPort, Client* clients)
{
	m_serverPort = gameServerPort;
	m_terminate = false;
	
	m_actionQueue = new Queue<Action*>();
	m_networkManager = new NetworkManager(m_terminate, gameServerPort, m_actionQueue);
	Queue<Event*>* eventQueue = new Queue<Event*>();
	m_ecs = new ECS(m_terminate, m_networkManager, eventQueue, clients);
//	m_logic = new Logic(m_terminate, clients, m_actionQueue, eventQueue);
}

GameServer::GameServer()
{

}
