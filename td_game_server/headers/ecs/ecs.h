#ifndef ECS_H__
#define ECS_H__

#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <atomic>

#include "vector2.h"
#include "path.h"
#include "event.h"
#include "client.h"
#include "networkManager.h"
#include "eventManager.h"

#include "entityCounter.h"
#include "componentArray.h"
//Components
#include "playerComponent.h"
#include "bankComponent.h"
#include "offenseComponent.h"
#include "motorComponent.h"
#include "transformComponent.h"

//Systems
#include "timeSystem.h"
#include "pathfindingSystem.h"
#include "rageSystem.h"
#include "movementSystem.h"

class ECS
{
public:
	ECS() {}
	~ECS();
	void Init(SharedQueue<Event*>& eventQueue, EntityCounter& entityCounter, Players& players, Banks& banks, Offenses& offenses, Motors& motors, Transforms& transforms);
	bool Loop();
private:
	void WaitForTerminate();
private:
	//Components
	Players* m_players;
	Banks* m_banks;
	Offenses* m_offenses;
	Motors* m_motors;
	Transforms* m_transforms;
	
	//Shared resources
	EntityCounter* m_entityCounter;
	SharedQueue<Event*>* m_eventQueue;

	//Systems
	TimeSystem m_timeSystem;
	PathfindingSystem m_pathfindingSystem;
	RageSystem m_rageSystem;
	MovementSystem m_movementSystem;
};


#endif
