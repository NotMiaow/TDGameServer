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
#include "eventTranslator.h"
#include "actionLanguage.h"
#include "queue.h"
#include "networkManager.h"

#include "eventManager.h"

class Logic
{
public:
	Logic(std::shared_future<void>&& serverFuture, Client* clients, Queue<Action*>* actionQueue, Queue<Event*>* eventQueue);
	Logic();
	~Logic();
	void ProcessActions(const int& threadID);
	void WaitForTerminate();
	void SwitchAction(Action* action);
	void HandleError(Action * action);
	void ConnectClient(Action* action);
	void DisconnectClient(Action* action);
	void RelayToEventManager(Action* action);
private:
	std::shared_future<void> m_serverFuture;
	std::atomic<bool> m_alive;
	std::thread m_terminateThread;
	std::thread m_threads[PROCESSING_THREADS_COUNT];
	Queue<Action*>* m_actionQueue;
	NetworkManager* m_networkManager;
	Client* m_clients;
	Queue<Event*>* m_eventQueue;
};

#endif