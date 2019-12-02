#ifndef EVENT_HANDLER_H__
#define EVENT_HANDLER_H__

#include <iostream>
#include <tuple>
#include <math.h>
#include <queue>

//Paths
#include "vector2.h"

//Events
#include "eventLanguage.h"
#include "event.h"
#include "shared_queue.h"

#include "checkpointList.h"
//Components
#include "client.h"
#include "playerComponent.h"
#include "bankComponent.h"
#include "offenseComponent.h"
#include "motorComponent.h"
#include "transformComponent.h"

//Networking
#include "networkManager.h"

//Misc
#include "gameEnums.h"

class EventManager
{
public:
    EventManager() { }
    ~EventManager();
    void Init(Client* clients, NetworkManager& networkManager, SharedQueue<Event*>& eventQueue, CheckpointList<PlayerComponent>& players, 
                CheckpointList<BankComponent>& banks, CheckpointList<OffenseComponent>& offenses, CheckpointList<MotorComponent>& motors,
                CheckpointList<TransformComponent>& transforms);
    void Loop();
private:
    void SwitchEvent();
    void ConnectPlayer();
    void DisconnectPlayer();
    void ReadyUpPlayer();
    void SpawnUnitGroup();
    void SendNewPath();
    void BuildTower();
    void SellTower();
    int GetPlayerPosition(const int& clientId, PlayerComponent* player);
private:
    Event* m_event;
    SharedQueue<Event*>* m_eventQueue;

   	CheckpointList<PlayerComponent>* m_players;
    CheckpointList<BankComponent>* m_banks;
    CheckpointList<OffenseComponent>* m_offenses;
   	CheckpointList<MotorComponent>* m_motors;
	CheckpointList<TransformComponent>* m_transforms;

    NetworkManager* m_networkManager;
};

#endif
