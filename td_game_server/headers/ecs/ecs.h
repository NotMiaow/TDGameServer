#ifndef ECS_H__
#define ECS_H__

#include <iostream>
#include <thread>

#include "checkpointList.h"
#include "eventHandler.h"
#include "addSystem.h"

class ECS
{
public:
	ECS();
	~ECS() { }
	void Loop();
private:
	bool m_alive;
	//components
	CheckpointList<int> m_ints;
	//systems
	AddSystem m_addSystem;
	//event handler
	EventHandler m_eventHandler;
};


#endif