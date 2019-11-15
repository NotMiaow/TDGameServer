#ifndef NETWORK_MANAGER_H__
#define NETWORK_MANAGER_H__

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <chrono>
#include <future>

#define DEFAULT_BUFLEN 512

#include "shared_queue.h"
#include "actionLanguage.h"
#include "action.h"
#include "actionTranslator.h"
#include "cst.h"

class NetworkManager
{
public:
	NetworkManager();
	NetworkManager(std::shared_future<void>&& serverFuture, const int serverPort, SharedQueue<Action*>& actionQueue);
	~NetworkManager();
	bool SetUpClientEnvironment(const int serverPort);
	void AcceptConnection(sockaddr_in& address);
	void ListenToClient(const int& socketId);
	void WaitForTerminate();
	void MessageClient(const int& socketId, std::string message);
	void KickClient(const int& socketId);
	void BroadCast(std::string& message);

private:
	std::shared_future<void> m_serverFuture;
	std::atomic<bool> m_alive;
	std::thread m_terminateThread;
	std::thread m_listeningThread;
	std::thread m_clientThreads[MAX_CLIENTS];

	int m_clientsPort;
	SharedQueue<Action*>* m_actionQueue;
	int m_listeningSocket;
	int m_clientSockets[MAX_CLIENTS];
	bool m_socketActive[MAX_CLIENTS];
};

#endif
