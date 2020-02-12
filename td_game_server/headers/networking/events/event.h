#ifndef EVENT_H__
#define	EVENT_H__

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <string>
#include <sstream>

//Networking
#include "sharedLanguage.h"
#include "eventLanguage.h"

//Libraries
#include "basicLib.h"
#include "vector2.h"

#include "componentArray.h"
//Components
#include "playerComponent.h"
#include "bankComponent.h"

//Misc
#include "definitions.h"
#include "cst.h"

struct Event
{
	virtual EventType GetType() const = 0;
	virtual std::string ToNetworkable() const = 0;

	int clientId;
	int playerId;
};

struct ErrorEvent : public Event
{
	ErrorEvent(const int& clientId, EventType eType, GameErrorType geType)
	{
		this->clientId = clientId;
		this->eType = eType;
		this->geType = geType;
	}
	EventType GetType() const { return EError; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << '{' << EError << ';' << eType << ';' << geType << '}';
		return os.str();
	}

	EventType eType;
	GameErrorType geType;
};

struct ConnectEvent : public Event
{
	ConnectEvent(const int&  clientId)
	{
		this->clientId = clientId;
	}
	EventType GetType() const { return EConnect; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << '{' << EConnect << '}';
		return os.str();
	}
};

struct DisconnectEvent : public Event
{
	DisconnectEvent(const int& clientId, DisconnectReason reason = RKicked)
	{
		this->clientId = clientId;
		this->reason = reason;
	}
	EventType GetType() const { return EDisconnect; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << '{' << EDisconnect << ';' << reason << '}';
		return os.str();
	}

	DisconnectReason reason;
};

struct ReadyUpEvent : public Event
{
	ReadyUpEvent(const int& clientId)
	{
		this->clientId = clientId;
		entityIds = new std::vector<int>();
	}
	~ReadyUpEvent() { delete entityIds; }
	EventType GetType() const { return EReadyUp; }
	std::string ToNetworkable() const
	{
		std::string message = '{' + std::to_string(EReadyUp) + ';' + std::to_string(playerId) + ';';
		std::vector<int>::iterator idIt = entityIds->begin();
		PlayerIterator playerIt = players->GetIterator();
		BankIterator bankIt = banks->GetIterator();
		for(; idIt != entityIds->end(); idIt++, playerIt++, bankIt++)
		{
			message += 	std::to_string(playerIt.GetData()->connected) + ';' + std::to_string(playerIt.GetData()->ready) + ';' +
						std::to_string(*idIt) + ';' + std::to_string(playerIt.GetData()->lives) + ';' +
						std::to_string(bankIt.GetData()->gold) + ';' + std::to_string(bankIt.GetData()->income) + ';';
		}
		message = message.substr(0, message.length() - 1);
		message += '}';
		return message;
	}

	std::vector<int>* entityIds;
	Players* players;
	Banks* banks;
};

struct SpawnUnitGroupEvent : public Event
{
	SpawnUnitGroupEvent(const int&  clientId)
	{
		this->clientId = clientId;
	}
	EventType GetType() const { return ESpawnUnitGroup; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << '{' << ESpawnUnitGroup << ';' << playerId << ';' << entityId << '}';
		return os.str();
	}

	int entityId;
};

struct NewPathEvent : public Event
{
	NewPathEvent() { path = new std::vector<Vector2>(); }
	~NewPathEvent() { delete path; }
	EventType GetType() const { return ENewPath; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << '{' << ENewPath << ';' << playerId << ';' << entityId << ';';
		for(std::vector<Vector2>::iterator pathIt = path->begin(); pathIt != path->end(); pathIt++)
			os << '(' << pathIt->y << ':' << pathIt->x << ')';
		os << '}';
		return os.str();
	}

	int entityId;
	std::vector<Vector2>* path;
};

struct RageEvent : public Event
{
	RageEvent() { }
	EventType GetType() const { return ERage; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << '{' << ERage << ';' << playerId << ';' << entityId << '}';
		return os.str();
	}

	int entityId;
};

struct BuildTowerEvent : public Event
{
	BuildTowerEvent();
	BuildTowerEvent(const int&  clientId, const int& towerType, const Vector2& position)
	{
		this->clientId = clientId;
		this->towerType = towerType;
		this->position = position;
	}
	EventType GetType() const { return EBuildTower; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << '{' << EBuildTower << ';' << playerId << ';' << remainingGold << ';' << entityId << ';' << towerType << ";(" << position.y << ':' << position.x << ")}";
		return os.str();
	}

	int entityId;
	int remainingGold;
	int towerType;
	Vector2 position;
};

struct SellTowerEvent : public Event
{
	SellTowerEvent(const int&  clientId, const int& entityId)
	{
		this->clientId = clientId;
		this->entityId = entityId;
	}
	EventType GetType() const { return ESellTower; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << '{' << ESellTower << ';' <<  playerId << ';' << entityId << ';' << remainingGold << '}';
		return os.str();
	}

	int playerId;
	int entityId;
	int remainingGold;
};

struct SendUnitGroupEvent : public Event
{
	SendUnitGroupEvent(const int& clientId, const int& unitType)
	{
		this->clientId = clientId;
		this->unitType = unitType;
	}
	EventType GetType() const { return ESendUnitGroup; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << '{' << ESendUnitGroup << '}';
	}

	int unitType;
};


#endif
