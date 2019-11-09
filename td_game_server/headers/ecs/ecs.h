#ifndef ECS_H__
#define ECS_H__

#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <atomic>

#include "queue.h"
#include "checkpointList.h"
#include "client.h"
#include "networkManager.h"
#include "eventManager.h"
#include "addSystem.h"
#include "event.h"

class ECS
{
public:
	ECS(std::shared_future<void>&& serverFuture, NetworkManager* networkmanager, Queue<Event*>* eventQueue, Client* clients, std::atomic<bool>& serverAlive);
	~ECS();
	void Loop();
	void WaitForTerminate();
private:
	std::shared_future<void> m_serverFuture;
	std::atomic<bool>* m_serverAlive;
	std::atomic<bool> m_alive;
	std::thread m_terminateThread;
	std::thread m_mainLoopThread;
	//components
	CheckpointList<int> m_ints;
	//systems
	AddSystem m_addSystem;
	//event handler
	EventManager m_eventManager;
};


#endif
