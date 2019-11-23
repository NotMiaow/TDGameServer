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

//Networking
#include "client.h"
#include "actionManager.h"
#include "networkManager.h"

//Components
#include "playerComponent.h"
#include "bankComponent.h"
#include "motorComponent.h"
#include "transformComponent.h"

//Misc
#include "cst.h"
#include "eventManager.h"
#include "ecs.h"

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
	CheckpointList<PlayerComponent> m_players;
	CheckpointList<BankComponent> m_banks;
	CheckpointList<MotorComponent> m_motors;
	CheckpointList<TransformComponent> m_transforms;

	//Misc
	EventManager m_eventManager;
	ECS m_ecs;
};
#endif