#ifndef LOGIC_H__
#define LOGIC_H__

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <future>

#include "basicLib.h"
#include "client.h"
#include "cst.h"
#include "action.h"
#include "event.h"
#include "actionTranslator.h"
#include "eventTranslator.h"
#include "actionLanguage.h"
#include "shared_queue.h"

#include "eventManager.h"

class ActionManager
{
public:
	ActionManager() { }
	ActionManager(Client* clients, SharedQueue<Action*>& actionQueue, SharedQueue<Event*>& eventQueue);
	~ActionManager();
	void Loop();
private:
	void SwitchAction(Action* action);
	void HandleError(Action * action);
	void ConnectClient(Action* action);
	void DisconnectClient(Action* action);
	void RelayToEventManager(Action* action);
private:
	Client* m_clients;
	SharedQueue<Action*>* m_actionQueue;
	SharedQueue<Event*>* m_eventQueue;
};

#endif