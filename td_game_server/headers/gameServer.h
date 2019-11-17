#ifndef GAME_SERVER_H__
#define GAME_SERVER_H__

#include <tuple>
#include <chrono>
#include <future>

#include "shared_queue.h"
#include "action.h"
#include "cst.h"
#include "networkManager.h"
#include "actionManager.h"
#include "ecs.h"
#include "client.h"

class GameServer
{
public:
	GameServer(const int& gameServerPort, Client* clients);
	GameServer();
	~GameServer();
	void Loop();
	void Stop();
private:
	void WaitForTerminate();
private:
	int m_serverPort;
	Client* m_clients;

	std::atomic<bool> m_alive;
	std::promise<void> m_exitSignal;
	std::shared_future<void> m_futureObj;
	std::thread m_terminateThread;
	std::thread m_mainLoopThread;

	SharedQueue<Event*> m_eventQueue;
	SharedQueue<Action*> m_actionQueue;
	NetworkManager* m_networkManager;
	ECS* m_ecs;
	ActionManager* m_actionManager;
};
#endif