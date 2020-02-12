#ifndef GAME_SERVER_H__
#define GAME_SERVER_H__

#include <tuple>
#include <chrono>
#include <future>
#include <thread>

//Shared resources
#include "shared_queue.h"
#include "event.h"
#include "action.h"

#include "entityCounter.h"
#include "componentArray.h"
//Components
#include "playerComponent.h"
#include "bankComponent.h"
#include "offenseComponent.h"
#include "motorComponent.h"
#include "transformComponent.h"

//Misc
#include "cst.h"
#include "ecs.h"
#include "eventManager.h"

//Networking
#include "client.h"
#include "actionManager.h"
#include "networkManager.h"

class GameServer
{
public:
	GameServer(const int& gameServerPort, Client* clients);
	~GameServer();
	void Loop();
	void Stop();
private:
	void WaitForTerminate();
private:
	//Terminate
	std::atomic<bool> m_alive;
	std::promise<void> m_exitSignal;
	std::shared_future<void> m_futureObj;
	std::thread m_terminateThread;
	std::thread m_mainLoopThread;
	
	//Shared resources
	SharedQueue<Event*> m_eventQueue;
	SharedQueue<Action*> m_actionQueue;

	//Networking
	int m_serverPort;
	Client* m_clients;
	NetworkManager m_networkManager;
	ActionManager m_actionManager;

	//Components
	Players m_players;
	Banks m_banks;
	Motors m_motors;
	Offenses m_offenses;
	Transforms m_transforms;

	//Misc
	EntityCounter m_entityCounter;
	EventManager m_eventManager;
	ECS m_ecs;
};
#endif