#ifndef ACTION_H__
#define ACTION_H__

#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <sstream>

#include "sharedLanguage.h"
#include "actionLanguage.h"

struct Action
{
	virtual ActionType GetType() const = 0;
	virtual std::string ToDebuggable() const = 0;

	int clientId;
	int socketId;
	std::string sessionToken;
};

struct ErrorAction : public Action
{
	ErrorAction(const int& socketId, const int& clientId, ActionType aType, NetworkingErrorType neType)
	{
		this->clientId = clientId;
		this->socketId = socketId;
		this->sessionToken = "";
		this->aType = aType;
		this->neType = neType;
	}
	ActionType GetType() const { return AError; }
	std::string ToDebuggable() const
	{
		std::ostringstream os;
		os << "{" << AError << ";" << aType << ";" << neType << ";" << clientId << ";" << socketId << "}";
		return os.str();
	}

	ActionType aType;
	NetworkingErrorType neType;
};

struct ConnectAction : public Action
{
	ConnectAction(const int& socketId, const int&  clientId, std::string sessionToken)
	{
		this->clientId = clientId;
		this->socketId = socketId;
		this->sessionToken = sessionToken;
	}
	ActionType GetType() const { return AConnect; }
	std::string ToDebuggable() const
	{
		std::ostringstream os;
		os << "{" << AConnect << "}";
		return os.str();
	}
};

struct DisconnectAction : public Action
{
	DisconnectAction(const int& socketId, DisconnectReason reason) : reason(reason)
	{
		this->socketId = socketId;
		this->clientId = -1;
		this->sessionToken = "";
	}
	ActionType GetType() const { return ADisconnect; }
	std::string ToDebuggable() const
	{
		std::ostringstream os;
		os << "{" << ADisconnect << ";" << reason << "}";
		return os.str();
	}

	DisconnectReason reason;
};

struct GameAction : public Action
{
	GameAction(const int& socketId, const int& clientId, const std::string& sessionToken, const std::string& gameEvent)
	{
		this->socketId = socketId;
		this->clientId = clientId;
		this->sessionToken = sessionToken;
		this->gameEvent = gameEvent;
	}
	ActionType GetType() const { return AGameAction; }
	std::string ToDebuggable() const
	{
		std::ostringstream os;
		os << "{" << AGameAction << ";" << gameEvent << "}";
		return os.str();
	}

	std::string gameEvent;
};

#endif