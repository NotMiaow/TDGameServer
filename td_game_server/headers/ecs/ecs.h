#ifndef ECS_H__
#define ECS_H__

#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <atomic>

#include "vector2.h"
#include "path.h"
#include "queue.h"
#include "checkpointList.h"
#include "event.h"
#include "client.h"
#include "networkManager.h"
#include "eventManager.h"

#include "motorComponent.h"
#include "transformComponent.h"

#include "timeSystem.h"
#include "movementSystem.h"

class ECS
{
public:
	ECS() {}
	ECS(NetworkManager* networkmanager, SharedQueue<Event*>& eventQueue, Client* clients, std::shared_future<void>&& serverFuture, std::atomic<bool>& serverAlive);
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
	CheckpointList<MotorComponent> m_motors;
	CheckpointList<TransformComponent> m_transforms;
	//systems
	TimeSystem m_timeSystem;
	MovementSystem m_movementSystem;
	//event handler
	EventManager m_eventManager;
};


#endif
