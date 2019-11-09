#ifndef NETWORK_MANAGER_H__
#define NETWORK_MANAGER_H__

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <chrono>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512

#include "actionLanguage.h"
#include "action.h"
#include "queue.h"
#include "actionTranslator.h"
#include "cst.h"

class NetworkManager
{
public:
	NetworkManager();
	NetworkManager(bool& terminate, const int serverPort, Queue<Action*>* actionQueue);
	~NetworkManager();
	bool SetUpClientEnvironment(const int serverPort);
	void AcceptConnection(sockaddr_in& address);
	void ListenToClient(const int& socketId);
	void MessageClient(const int& socketId, std::string message);
	void KickClient(const int& socketId);
	void BroadCast(std::string& message);

private:
	bool m_terminate;
	int m_clientsPort;
	Queue<Action*>* m_actionQueue;
	int m_listeningSocket;
	std::thread m_listeningThread;
	int m_clientSockets[MAX_CLIENTS];
	std::thread m_clientThreads[MAX_CLIENTS];
	bool m_socketActive[MAX_CLIENTS];
};

#endif
