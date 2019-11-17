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

#include "playerComponent.h"
#include "motorComponent.h"
#include "transformComponent.h"

#include "timeSystem.h"
#include "movementSystem.h"

class ECS
{
public:
	ECS() {}
	ECS(NetworkManager* networkManager, SharedQueue<Event*>& eventQueue, Client* clients);
	~ECS();
	bool Loop();
private:
	void WaitForTerminate();
private:
	//Components
	CheckpointList<PlayerComponent> m_players;
	CheckpointList<MotorComponent> m_motors;
	CheckpointList<TransformComponent> m_transforms;
	
	//Systems
	TimeSystem m_timeSystem;
	MovementSystem m_movementSystem;
	
	//Event mang
	EventManager m_eventManager;

	//Networking
	NetworkManager* m_networkManager;
};


#endif
