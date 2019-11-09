#ifndef GAME_SERVER_H__
#define GAME_SERVER_H__

#include <vector>
#include <tuple>

#include "action.h"
#include "queue.h"
#include "networkManager.h"
#include "logic.h"
#include "ecs.h"
#include "client.h"

class GameServer
{
public:
	GameServer(const int& gameServerPort, Client* clients);
	GameServer();
private:
	int m_serverPort;
	bool m_terminate;
	Queue<Action*>* m_actionQueue;
	NetworkManager* m_networkManager;
	ECS* m_ecs;
	Logic* m_logic;
};
#endif