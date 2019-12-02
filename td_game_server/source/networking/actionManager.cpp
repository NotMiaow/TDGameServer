#include "actionManager.h"

ActionManager::~ActionManager()
{

}

void ActionManager::Init(Client* clients, SharedQueue<Action*>& actionQueue, SharedQueue<Event*>& eventQueue) {
	m_clients = clients;
	m_actionQueue = &actionQueue;
	m_eventQueue = &eventQueue;
}

void ActionManager::Loop()
{
	if (m_actionQueue->size())
	{
		Action* a = m_actionQueue->front();
		std::cout << a->ToDebuggable() << std::endl;
		if(a != 0) SwitchAction(a);
		delete a;
		m_actionQueue->pop_front();
	}
}

void ActionManager::SwitchAction(Action* action)
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
}

void ActionManager::HandleError(Action* action)
{
	std::cout << "error" << action->ToDebuggable() << std::endl;
}

void ActionManager::ConnectClient(Action* action)
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
					m_eventQueue->push_back(disconnectEvent);
				}

				m_clients[i].socketId = a->socketId;
				Event* connectEvent = CreateConnectEvent(a->clientId);
				m_eventQueue->push_back(connectEvent);
				std::cout << "Client with id \"" << a->clientId << "\" and token \"" << a->sessionToken << "\" connected on socket \"" << a->socketId << "\"" << std::endl;
				return;
			}
			else
			{
				std::cout << "Invalid session token, kicking client." << std::endl;
				Action* errorAction = CreateErrorAction(a->socketId, a->clientId, AConnect, NEWrongSessionToken);
				m_actionQueue->push_back(errorAction);
				Event* disconnectEvent = CreateDisconnectEvent(a->clientId);
				m_eventQueue->push_back(disconnectEvent);
				m_clients[i].socketId = -1;
				return;
			}
		}
	}
}

void ActionManager::DisconnectClient(Action* action)
{
	DisconnectAction* a = dynamic_cast<DisconnectAction*>(action);
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (m_clients[i].socketId == a->socketId)
		{
			std::cout << "Client disconnected from socket: " << a->socketId << std::endl;
			Event* disconnectEvent = CreateDisconnectEvent(a->clientId, a->reason);
			m_eventQueue->push_back(disconnectEvent);
			m_clients[i].socketId = -1;
		}
	}
}

void ActionManager::RelayToEventManager(Action * action)
{
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (m_clients->id == action->clientId)
		{
			if (m_clients->token == action->sessionToken)
			{
				GameAction* a = dynamic_cast<GameAction*>(action);
				Event* e = CreateGameEvent(a->clientId, Split(a->gameEvent, (int)a->gameEvent.length()));
				m_eventQueue->push_back(e);
				return;
			}
			else
			{
				Action* e = CreateErrorAction(action->socketId, action->clientId, AGameAction, NEWrongSessionToken);
				m_actionQueue->push_back(e);
				return;
			}
		}
	}
}
