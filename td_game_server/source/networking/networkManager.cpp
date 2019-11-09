#include "networkManager.h"

NetworkManager::NetworkManager(bool& terminate, const int serverPort, Queue<Action*>* actionQueue)
{
	m_clientsPort = serverPort;
	m_actionQueue = actionQueue;
	m_terminate = terminate;

	for (int i = 0; i < MAX_CLIENTS; i++)
		m_socketActive[i] = false;

	if (!SetUpClientEnvironment(serverPort))
		exit(EXIT_FAILURE);
}

NetworkManager::~NetworkManager()
{
	if (m_listeningThread.joinable())
		m_listeningThread.join();
}


bool NetworkManager::SetUpClientEnvironment(const int serverPort)
{
	sockaddr_in address;
	int opt = 1;

	if((m_listeningSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		std::cout << "Failed at creating socket file descriptor." << std::endl;
		return false;
	}

	if(setsockopt(m_listeningSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		std::cout << "Failed at setsocketopt." << std::endl;
		return false;
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(serverPort);

	if (bind(m_listeningSocket, (sockaddr*)&address, sizeof(address)) < 0) 
    { 
        std::cout << "Failed at bind." << std::endl; 
        return false;
    } 

    if (listen(m_listeningSocket, 3) < 0) 
    { 
        std::cout << "Failed at listen." << std::endl;; 
		return false;
    }

	m_listeningThread = std::thread(&NetworkManager::AcceptConnection, this, std::ref(address));
	return true;
}

void NetworkManager::AcceptConnection(sockaddr_in& address)
{
	while (!m_terminate)
	{
		for (int i = 0; i < MAX_CLIENTS; i++)
		{
			if (m_socketActive[i])
				continue;

			int addrlen = sizeof(address);
			if((m_clientSockets[i] = accept(m_listeningSocket, (sockaddr*)&address, (socklen_t*)&addrlen)) < 0)
			{
				i--;
				continue;
			}

			m_socketActive[i] = true;
			if (m_clientThreads[i].joinable()) m_clientThreads[i].join();
			m_clientThreads[i] = std::thread(&NetworkManager::ListenToClient, this, i);
		}
	}

	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		m_clientThreads[i].join();
		close(m_clientSockets[i]);
	}
	close(m_listeningSocket);
}

void NetworkManager::ListenToClient(const int& socketId)
{
	int iResult = 0;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	do {
		iResult = read(m_clientSockets[socketId], recvbuf, recvbuflen);
		if (iResult > 0)
		{
			Action* action = CreateAction(socketId, Split(recvbuf, iResult));
			m_actionQueue->QueueUp(action);
		}
		else {
			Action* disconnectAction = CreateDisconnectAction(socketId);
			m_actionQueue->QueueUp(disconnectAction);
			m_socketActive[socketId] = false;
			return;
		}
	} while (iResult > 0 && !m_terminate);
	m_socketActive[socketId] = false;

	Action* disconnectAction = CreateDisconnectAction(socketId);
	m_actionQueue->QueueUp(disconnectAction);
	iResult = shutdown(m_clientSockets[socketId], SHUT_WR);
}

void NetworkManager::MessageClient(const int& socketId, std::string message)
{
	int iSendResult = send(m_clientSockets[socketId], message.c_str(), (int)message.length(), 0);
	if (iSendResult <= 0) KickClient(socketId);
}

void NetworkManager::KickClient(const int & socketId)
{
	close(m_clientSockets[socketId]);
	m_socketActive[socketId] = false;
}
