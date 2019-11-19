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
#include "motorComponent.h"
#include "transformComponent.h"

//Networking
#include "networkManager.h"

class EventManager
{
public:
    EventManager() { }
    EventManager(NetworkManager* networkManger, Client* clients, SharedQueue<Event*>& eventQueue, CheckpointList<PlayerComponent>& players, CheckpointList<MotorComponent>& motors, 
                CheckpointList<TransformComponent>& transforms);
    ~EventManager();
    void Loop();
private:
    void SwitchEvent();
    void ConnectPlayer();
    void ReadyUpPlayer();
    CheckpointList<PlayerComponent>::Node<PlayerComponent>* FindPlayerByClientId(const int& clientId);
private:
    Event* m_event;
    SharedQueue<Event*>* m_eventQueue;

   	CheckpointList<PlayerComponent>* m_players;
   	CheckpointList<MotorComponent>* m_motors;
	CheckpointList<TransformComponent>* m_transforms;

    NetworkManager* m_networkManager;
};

#endif
