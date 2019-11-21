#ifndef EVENT_HANDLER_H__
#define EVENT_HANDLER_H__

#include <iostream>
#include <math.h>

//Paths
#include "vector2.h"
#include "queue.h"

//Events
#include "eventLanguage.h"
#include "event.h"
#include "shared_queue.h"

#include "checkpointList.h"
//Components
#include "client.h"
#include "playerComponent.h"
#include "bankComponent.h"
#include "motorComponent.h"
#include "transformComponent.h"

//Networking
#include "networkManager.h"

class EventManager
{
public:
    EventManager() { }
    EventManager(NetworkManager* networkManager, Client* clients, SharedQueue<Event*>& eventQueue, CheckpointList<PlayerComponent>& players, 
                CheckpointList<BankComponent>& resources, CheckpointList<MotorComponent>& motors, CheckpointList<TransformComponent>& transforms);
    ~EventManager();
    void Loop();
private:
    void SwitchEvent();
    void ConnectPlayer();
    void DisconnectPlayer();
    void ReadyUpPlayer();
    const int FindPlayerByClientId(const int &clientId, CheckpointList<PlayerComponent>::Node<PlayerComponent> * pit);
    const int GetPlayerBank(const int& clientId, CheckpointList<BankComponent>::Node<BankComponent>* bit);
private:
    Event* m_event;
    SharedQueue<Event*>* m_eventQueue;

   	CheckpointList<PlayerComponent>* m_players;
    CheckpointList<BankComponent>* m_banks;
   	CheckpointList<MotorComponent>* m_motors;
	CheckpointList<TransformComponent>* m_transforms;

    NetworkManager* m_networkManager;
};

#endif
