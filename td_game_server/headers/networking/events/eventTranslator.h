#ifndef	EVENT_TRANSLATOR_H__
#define EVENT_TRANSLATOR_H__

#include <string>
#include <vector>

#include "basicLib.h"
#include "event.h"
#include "eventLanguage.h"
#include "gameEnums.h"

static Event* CreateErrorEvent(const int &clientId, EventType aType, GameErrorType eType)
{
	Event* e = new ErrorEvent(clientId, aType, eType);
	return e;
}

static Event* CreateConnectEvent(const int & clientId)
{
	Event *e = new ConnectEvent(clientId);
	return e;
}

static Event* CreateDisconnectEvent(const int& clientId, DisconnectReason reason = RKicked)
{
	Event *e = new DisconnectEvent(clientId, reason);
	return e;
}

static Event* CreateReadyUpEvent(const int& clientId)
{
	Event* e = new ReadyUpEvent(clientId);
	return e;
}

static Event* CreateBuildTowerEvent(const int& clientId, std::vector<std::string> elements)
{
	int towerType;
	Vector2 position;

	if (elements.size() != 2) return CreateErrorEvent(clientId, EBuildTower, GEWrongParemeterAmount);
	if (!ToInt(elements[0], towerType)) return CreateErrorEvent(clientId, EBuildTower, GEWrongParameterType);
	if(!ToPosition(elements[1], position)) return CreateErrorEvent(clientId, EBuildTower, GEWrongParameterType);

	Event* e = new BuildTowerEvent(clientId, towerType, position);
	return e;
}

static Event* CreateSellTowerEvent(const int& clientId, std::vector<std::string> elements)
{
	int towerPosition;

	if (elements.size() != 1) return CreateErrorEvent(clientId, ESellTower, GEWrongParemeterAmount);
	if (!ToInt(elements[0], towerPosition)) return CreateErrorEvent(clientId, ESellTower, GEWrongParameterType);

	Event* e = new SellTowerEvent(clientId, towerPosition);
	return e;
}


static Event* CreateSendUnitGroupEvent(const int& clientId, std::vector<std::string> elements)
{
	int unitType;

	if (elements.size() != 1) return CreateErrorEvent(clientId, ESendUnitGroup, GEWrongParemeterAmount);
	if (!ToInt(elements[0], unitType)) return CreateErrorEvent(clientId, ESendUnitGroup, GEWrongParameterType);

	Event* e = new SendUnitGroupEvent(clientId, unitType);
	return e;
}

static Event* CreateGameEvent(const int & clientId, std::vector<std::string> elements)
{
	int eventType = -1;
	bool canInt = ToInt(elements[0], eventType);

	elements.erase(elements.begin());
	
	if (canInt)
	{
		switch (eventType)
		{
		case EConnect:
			return CreateConnectEvent(clientId);
		case EDisconnect:
			return CreateDisconnectEvent(clientId);
		case EReadyUp:
			return CreateReadyUpEvent(clientId);
		case ESpawnUnitGroup:
			//This event is send only
			break;
		case ENewPath:
			//This event is send only
			break;
		case ERage:
			//This event is send only
			break;
		case EBuildTower:
			return CreateBuildTowerEvent(clientId, elements);
		case ESellTower:
			return CreateSellTowerEvent(clientId, elements);
		case ESendUnitGroup:
			return CreateSendUnitGroupEvent(clientId, elements); //This event is receive only
		default:
			return CreateErrorEvent(clientId, ECreateEvent, GEWrongEventType);
		}
	}
	return CreateErrorEvent(clientId, ECreateEvent, GEWrongEventType);
}


#endif