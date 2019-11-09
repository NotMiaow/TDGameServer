#ifndef ECS_H__
#define ECS_H__

#include <iostream>
#include <thread>

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
	ECS(bool& terminate, NetworkManager* networkmanager, Queue<Event*>* eventQueue, Client* clients);
	~ECS() { }
	void Loop();
private:
	bool m_alive;
	//components
	CheckpointList<int> m_ints;
	//systems
	AddSystem m_addSystem;
	//event handler
	EventManager m_eventManager;
};


#endif
