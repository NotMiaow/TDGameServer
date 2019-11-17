#ifndef LOGIC_H__
#define LOGIC_H__

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <future>

#include "client.h"
#include "cst.h"
#include "action.h"
#include "event.h"
#include "actionTranslator.h"
#include "eventTranslator.h"
#include "actionLanguage.h"
#include "shared_queue.h"

#include "eventManager.h"

class Logic
{
public:
	Logic() { }
	Logic(std::shared_future<void>&& serverFuture, Client* clients, SharedQueue<Action*>& actionQueue, SharedQueue<Event*>& eventQueue);
	~Logic();
	void ProcessActions(const int& threadID);
	void WaitForTerminate();
	void SwitchAction(Action* action);
	void HandleError(Action * action);
	void ConnectClient(Action* action);
	void DisconnectClient(Action* action);
	void RelayToEventManager(Action* action);
private:
	//Terminate
	std::shared_future<void> m_serverFuture;
	std::atomic<bool> m_alive;
	std::thread m_terminateThread;

	//Processing actions
	std::thread m_threads[PROCESSING_THREADS_COUNT];
	Client* m_clients;
	SharedQueue<Action*>* m_actionQueue;

	//Creating events
	SharedQueue<Event*>* m_eventQueue;
};

#endif