#ifndef EVENT_H__
#define	EVENT_H__

#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <sstream>

//Networking
#include "sharedLanguage.h"
#include "eventLanguage.h"

//Libraries
#include "basicLib.h"
#include "vector2.h"
#include "queue.h"

#include "checkpointList.h"
//Components
#include "playerComponent.h"
#include "bankComponent.h"

//Misc
#include "cst.h"

struct Event
{
	virtual EventType GetType() const = 0;
	virtual std::string ToNetworkable() const = 0;

	int clientId;
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
		os << "{" << EError << ";" << eType << ";" << geType << "}";
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
		os << "{" << EConnect << "}";
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
		os << "{" << EDisconnect << ";" << reason << "}";
		return os.str();
	}

	DisconnectReason reason;
};

struct ReadyUpEvent : public Event
{
	ReadyUpEvent(const int&  clientId)
	{
		this->clientId = clientId;
	}
	EventType GetType() const { return EReadyUp; }
	std::string ToNetworkable() const
	{
		CheckpointList<PlayerComponent>::Iterator playerIt(players->GetNodeHead(), 0);
		CheckpointList<BankComponent>::Iterator bankIt(banks->GetNodeHead(), 0);
		PlayerComponent* player;
		BankComponent* bank;
		std::string message = "{" + std::to_string(EReadyUp) + ";" + std::to_string(playerPosition) + ";";
		for(;!playerIt.End(); playerIt++, bankIt++)
		{
			player = playerIt.Get();
			bank =   bankIt.Get();
			message += std::to_string(player->connected) + ";" + std::to_string(player->ready) + ";" + std::to_string(player->lives) + ";" + 
						std::to_string(bank->gold) + ";" + std::to_string(bank->income) + ";";
		}
		message = message.substr(0, message.length() - 2);
		message += "}";
		return message;
	}

	int playerPosition;
	CheckpointList<PlayerComponent>* players;
	CheckpointList<BankComponent>* banks;
};

struct SpawnUnitGroupEvent : public Event
{
	SpawnUnitGroupEvent() { }
	EventType GetType() const { return ESpawnUnitGroup; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << ESpawnUnitGroup << "}";
		return os.str();
	}
};

struct NewPathEvent : public Event
{
	NewPathEvent() { }
	EventType GetType() const { return ENewPath; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << ENewPath << ";" << motorPosition << ";";
		//APPEND POSITIONS
		std::cout << "APPEND POSITIONS TO NewPathEvent.ToNetworkable()" << std::endl;
		os << "}";
		return os.str();
	}

	int motorPosition;
	Queue<Vector2> path;
};

struct RageEvent : public Event
{
	RageEvent() { }
	EventType GetType() const { return ERage; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << ERage << ";" << motorPosition << "}";
		return os.str();
	}

	int motorPosition;
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
		os << "{" << EBuildTower << ";" << remainingGold << ";" << towerType << ";(" << position.y << ":" << position.x << ")}";
		return os.str();
	}

	int remainingGold;
	int towerType;
	Vector2 position;
};

struct SellTowerEvent : public Event
{
	SellTowerEvent(const int&  clientId, const Vector2& towerPosition)
	{
		this->clientId = clientId;
		this->towerPosition.x = towerPosition.x;
		this->towerPosition.y = towerPosition.y;
	}
	EventType GetType() const { return ESellTower; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << ESellTower << ";(" << towerPosition.y << ":" << towerPosition.x << ");" << remainingGold << "}";
		return os.str();
	}

	Vector2 towerPosition;
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
		os << "{" << ESendUnitGroup << "}";
	}

	int unitType;
};


#endif
