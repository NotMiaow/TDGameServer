#include "logic.h"

Logic::Logic(bool & terminate, Client* clients, Queue<Action*>* actionQueue, Queue<Event*>* eventQueue) {
	m_terminate = terminate;
	m_actionQueue = actionQueue;
	m_eventQueue = eventQueue;

	m_clients = clients;

	for (int i = 0; i < PROCESSING_THREADS_COUNT; i++)
		m_threads[i] = std::thread(&Logic::ProcessActions, this, i);
}

Logic::Logic()
{

}

Logic::~Logic()
{
	for (int i = 0; i < PROCESSING_THREADS_COUNT; i++)
		m_threads[i].join();
}

void Logic::ProcessActions(const int& threadID)
{
	while (!m_terminate)
	{
		Action* a = m_actionQueue->GetComponent();
		if (a != nullptr)
			SwitchAction(a);
	}
}

void Logic::SwitchAction(Action* action)
{
	switch (action->GetType())
	{
	case AError:
		HandleError(action);
		break;
	case AConnect:
		ConnectClient(action);
		break;
	case ADisconnect:
		DisconnectClient(action);
		break;
	case AGameAction:
		RelayToEventManager(action);
		break;
	}

	delete action;
}

void Logic::HandleError(Action* action)
{
}

void Logic::ConnectClient(Action* action)
{
	ConnectAction* a = dynamic_cast<ConnectAction*>(action);
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (m_clients[i].id == a->clientId)
		{
			if (m_clients[i].token == a->sessionToken)
			{
				if (m_clients[i].socketId != -1)
				{
					std::cout << "Kicking client with id: \"" << m_clients[i].id << "\" on socket \"" << m_clients[i].socketId << "\"" << std::endl;
					Event* disconnectEvent = CreateDisconnectEvent(a->clientId, RNewLogin);
					m_eventQueue->QueueUp(disconnectEvent);
				}

				m_clients[i].socketId = a->socketId;
				Event* connectEvent = CreateConnectEvent(a->clientId);
				m_eventQueue->QueueUp(connectEvent);
				std::cout << "Client with id \"" << a->clientId << "\" and token \"" << a->sessionToken << "\" connected on socket \"" << a->socketId << "\"" << std::endl;
				return;
			}
			else
			{
				std::cout << "Invalid session token, kicking client." << std::endl;
				Action* errorAction = CreateErrorAction(a->socketId, a->clientId, AConnect, NEWrongSessionToken);
				m_actionQueue->QueueUp(errorAction);
				Event* disconnectEvent = CreateDisconnectEvent(a->clientId);
				m_eventQueue->QueueUp(disconnectEvent);
				m_clients[i].socketId = -1;
				return;
			}
		}
	}
}

void Logic::DisconnectClient(Action* action)
{
	DisconnectAction* a = dynamic_cast<DisconnectAction*>(action);
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (m_clients[i].socketId == a->socketId)
		{
			std::cout << "Client disconnected from socket: " << a->socketId << std::endl;
			Event* disconnectEvent = CreateDisconnectEvent(a->clientId, a->reason);
			m_eventQueue->QueueUp(disconnectEvent);
			m_clients[i].socketId = -1;
		}
	}
}

void Logic::RelayToEventManager(Action * action)
{
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (m_clients->id == action->clientId)
		{
			if (m_clients->token == action->sessionToken)
			{
				GameAction* a = dynamic_cast<GameAction*>(action);
				Event* e = CreateGameEvent(a->clientId, Split(a->gameEvent, (int)a->gameEvent.length()));
				m_eventQueue->QueueUp(e);
				return;
			}
			else
			{
				Action* e = CreateErrorAction(action->socketId, action->clientId, AGameAction, NEWrongSessionToken);
				m_actionQueue->QueueUp(e);
				return;
			}
		}
	}
}
