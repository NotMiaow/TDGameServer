#ifndef ACTION_TRANSLATOR_H__
#define ACTION_TRANSLATOR_H__

#include <iostream>
#include <string>
#include <vector>

#include "basicLib.h"
#include "action.h"
#include "actionLanguage.h"

static Action* CreateErrorAction(const int& socketId, const int &clientId, ActionType aType, NetworkingErrorType eType)
{
	Action* action = new ErrorAction(socketId, clientId, aType, eType);
	return action;
}

static Action* CreateConnectAction(const int & socketId, const std::vector<std::string>& elements)
{
	int clientId = -1;

	if (elements.size() != 2) return CreateErrorAction(socketId, -1, AConnect, NEWrongParemeterAmount);
	if (!ToInt(elements[0], clientId)) return CreateErrorAction(socketId, -1, AConnect, NEWrongParameterType);

	Action *action = new ConnectAction(socketId, clientId, elements[1]);
	return action;
}

static Action* CreateDisconnectAction(const int& socketId, DisconnectReason reason = RKicked)
{
	Action *action = new DisconnectAction(socketId, reason);
	return action;
}

static Action* CreateGameAction(const int& socketId, std::vector<std::string> elements)
{
	int clientId = -1;

	if (elements.size() != 3) return CreateErrorAction(socketId, -1, AConnect, NEWrongParemeterAmount);
	if (!ToInt(elements[0], clientId)) return CreateErrorAction(socketId, -1, AConnect, NEWrongParameterType);

	Action *action = new GameAction(socketId, clientId, elements[1], elements[2]);
	return action;
}


static Action* CreateAction(int const& socketId, std::vector<std::string> elements)
{
	if (elements.size() < 1) return CreateErrorAction(socketId, -1, ACreateAction, NEWrongParemeterAmount);

	int actionType = -1;
	bool canInt = ToInt(elements[0], actionType);
	elements.erase(elements.begin());

	if (canInt)
	{
		switch (actionType)
		{
		case AConnect:
			return CreateConnectAction(socketId, elements);
		case ADisconnect:
			return CreateDisconnectAction(socketId);
		case AGameAction:
			return CreateGameAction(socketId, elements);
		default:
			return CreateErrorAction(socketId, -1, ACreateAction, NEWrongActionType);
		}
	}
	return CreateErrorAction(socketId, -1, ACreateAction, NEWrongActionType);
}

#endif