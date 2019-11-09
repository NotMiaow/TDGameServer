#ifndef LOGIC_H__
#define LOGIC_H__

#include <iostream>
#include <string>
#include <thread>

#include "client.h"
#include "action.h"
#include "event.h"
#include "eventTranslator.h"
#include "actionLanguage.h"
#include "queue.h"
#include "constants.h"
#include "networkManager.h"

#include "eventManager.h"

class Logic
{
public:
	Logic(bool& terminate, Client* clients, Queue<Action*>* actionQueue, Queue<Event*>* eventQueue);
	Logic();
	~Logic();
	void ProcessActions(const int& threadID);
	void SwitchAction(Action* action);
	void HandleError(Action * action);
	void ConnectClient(Action* action);
	void DisconnectClient(Action* action);
	void RelayToEventManager(Action* action);
private:
	bool m_terminate;
	Queue<Action*>* m_actionQueue;
	NetworkManager* m_networkManager;
	Client* m_clients;
	std::thread m_threads[PROCESSING_THREADS_COUNT];
	Queue<Event*>* m_eventQueue;
};

#endif