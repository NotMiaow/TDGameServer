#ifndef ECS_H__
#define ECS_H__

#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <atomic>

#include "vector2.h"
#include "path.h"
#include "checkpointList.h"
#include "event.h"
#include "client.h"
#include "networkManager.h"
#include "eventManager.h"

#include "playerComponent.h"
#include "bankComponent.h"
#include "offenseComponent.h"
#include "motorComponent.h"
#include "transformComponent.h"

#include "timeSystem.h"
#include "movementSystem.h"
#include "pathfindingSystem.h"

class ECS
{
public:
	ECS() {}
	~ECS();
	void Init(SharedQueue<Event*>& eventQueue, CheckpointList<PlayerComponent>& players, CheckpointList<BankComponent>& banks,
			CheckpointList<OffenseComponent>& offenses, CheckpointList<MotorComponent>& motors, CheckpointList<TransformComponent>& transforms);
	bool Loop();
private:
	void WaitForTerminate();
private:
	//Components
	CheckpointList<PlayerComponent>* m_players;
	CheckpointList<BankComponent>* m_banks;
	CheckpointList<OffenseComponent>* m_offenses;
	CheckpointList<MotorComponent>* m_motors;
	CheckpointList<TransformComponent>* m_transforms;
	
	//Shared resources
	SharedQueue<Event*>* m_eventQueue;

	//Systems
	TimeSystem m_timeSystem;
	PathfindingSystem m_pathfindingSystem;
	MovementSystem m_movementSystem;
};


#endif
