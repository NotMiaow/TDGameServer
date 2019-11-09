#ifndef GAME_SERVER_H__
#define GAME_SERVER_H__

#include <tuple>
#include <chrono>
#include <future>

#include "action.h"
#include "queue.h"
#include "cst.h"
#include "networkManager.h"
#include "logic.h"
#include "ecs.h"
#include "client.h"

class GameServer
{
public:
	GameServer(const int& gameServerPort, Client* clients);
	GameServer();
	~GameServer();
	void WaitForTerminate();
	void Stop();
private:
	int m_serverPort;

	std::atomic<bool> m_alive;
	std::thread m_terminateThread;

	std::promise<void> m_exitSignal;
	std::shared_future<void> m_futureObj;

	Queue<Action*>* m_actionQueue;
	NetworkManager* m_networkManager;
	ECS* m_ecs;
	Logic* m_logic;
};
#endif