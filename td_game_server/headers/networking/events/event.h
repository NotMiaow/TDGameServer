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
		DataNode<PlayerComponent>* pit = players->GetNodeHead();
		DataNode<BankComponent>* bit = banks->GetNodeHead();
		std::ostringstream os;
		os << "{" << EReadyUp << ";" << playerPosition << ";";
		while (pit->next)
		{
			os << pit->data.connected << ";" << pit->data.ready << ";" << pit->data.lives << ";" << bit->data.gold << ";" << bit->data.income << ";";
			pit = players->GetNextNode(&*pit);
			bit = banks->GetNextNode(&*bit);
		}
		os << pit->data.connected << ";" << pit->data.ready << ";" << pit->data.lives << ";" << bit->data.gold << ";" << bit->data.income << "}";
		return os.str();
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
	SellTowerEvent(const int&  clientId, const int& towerPosition)
	{

		this->clientId = clientId;
		this->towerPosition = towerPosition;
	}
	EventType GetType() const { return ESellTower; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << ESellTower << ";" << towerPosition << ";" << sellValue << "}";
		return os.str();
	}

	int towerPosition;
	int sellValue;
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
