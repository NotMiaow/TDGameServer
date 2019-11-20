#include "eventManager.h"

EventManager::EventManager(NetworkManager *networkManager, Client *clients, SharedQueue<Event *> &eventQueue, CheckpointList<PlayerComponent> &players,
                           CheckpointList<ResourceComponent> &resources, CheckpointList<MotorComponent> &motors, CheckpointList<TransformComponent> &transforms)
{
    m_networkManager = networkManager;
    m_eventQueue = &eventQueue;

    m_players = &players;
    m_resources = &resources;
    m_motors = &motors;
    m_transforms = &transforms;

    //Seed players
    CheckpointList<PlayerComponent>::Node<PlayerComponent> *pit = m_players->GetNodeHead();
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        PlayerComponent player;
        player.client = &clients[i];
        player.connected = false;
        player.ready = false;
        player.lives = STARTING_LIVES;
        pit = m_players->InsertAfterNode(player, pit);
    }

    CheckpointList<ResourceComponent>::Node<ResourceComponent> *rit = m_resources->GetNodeHead();
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        ResourceComponent resource;
        resource.gold = STARTING_GOLD;
        resource.income = STARTING_INCOME;
        rit = m_resources->InsertAfterNode(resource, rit);
    }

    //Create Motor
    //    MotorComponent motor;
    //    Queue<Vector2> path(30);
    //    Vector2 target1;
    //    Vector2 target2;
    //    Vector2 target3;
    //    Vector2 target4;
    //    target1.x = 8;
    //    target1.y = 3;
    //    target2.x = 2;
    //    target2.y = 6;
    //    target3.x = 10;
    //    target3.y = 8;
    //    target4.x = 2;
    //    target4.y = GRID_SIZE_Y + DESPAWN_SIZE;
    //    path.Push(target4);
    //    path.Push(target3);
    //    path.Push(target2);
    //    path.Push(target1);
    //    motor.path = path;
    //    motor.behaviour = Move;
    //    motor.baseSpeed = 2;
    //    motor.curSpeed = 2;
    //    m_motors->InsertAfterNode(motor, m_motors->GetNodeHead());
    //
    //    //Create Transform
    //    TransformComponent transform;
    //    transform.position.x = SPAWN_POSITION_X;
    //    transform.position.y = SPAWN_POSITION_Y;
    //    //Update normalised target
    //	float distanceX = motor.path.Front().x - transform.position.x;
    //	float distanceY = motor.path.Front().y - transform.position.y;
    //	float distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));
    //	transform.normalizedTarget.x = distanceX / distance;
    //	transform.normalizedTarget.y = distanceY / distance;
    //    m_transforms->InsertAfterNode(transform, m_transforms->GetNodeHead());
}

EventManager::~EventManager()
{
}

void EventManager::Loop()
{
    while (m_eventQueue->GetSize())
    {
        m_event = m_eventQueue->Pop();
        if (m_event != 0)
            SwitchEvent();
        delete m_event;
    }
}

void EventManager::SwitchEvent()
{
    switch (m_event->GetType())
    {
    case EError:
        break;
    case EConnect:
        ConnectPlayer();
        break;
    case EDisconnect:
        break;
    case EReadyUp:
        ReadyUpPlayer();
        break;
    case ESpawnUnitGroup:
        break;
    case ENewPath:
        break;
    case ERage:
        break;
    case EBuildTower:
        break;
    case ESellTower:
        break;
    case ESendUnitGroup:
        break;
    }
}

void EventManager::ConnectPlayer()
{
    m_event = dynamic_cast<ConnectEvent *>(m_event);
    CheckpointList<PlayerComponent>::Node<PlayerComponent> *pit = m_players->GetNodeHead();
    FindPlayerByClientId(m_event->clientId, pit);

    if (pit != 0)
    {
        pit->data.connected = true;
        m_networkManager->MessageClient(pit->data.client->socketId, m_event->ToNetworkable());
    }
}

void EventManager::ReadyUpPlayer()
{
    m_event = dynamic_cast<ReadyUpEvent *>(m_event);
    CheckpointList<PlayerComponent>::Node<PlayerComponent> *pit = m_players->GetNodeHead();
    const int playerPosition = FindPlayerByClientId(m_event->clientId, pit);

    if (pit)
    {
        pit->data.ready = true;
        ((ReadyUpEvent *)m_event)->playerPosition = playerPosition;
        ((ReadyUpEvent *)m_event)->resources = m_resources;
        m_networkManager->MessageClient(pit->data.client->socketId, m_event->ToNetworkable());
        return;
    }
}

const int EventManager::FindPlayerByClientId(const int &clientId, CheckpointList<PlayerComponent>::Node<PlayerComponent> * pit = 0)
{
    int playerPosition = 0;
    if(pit == 0) pit = m_players->GetNodeHead();
    while (pit->data.client->id != clientId && pit != 0)
    {
        playerPosition++;
        pit = m_players->GetNextNode(&*pit);
    }
    return playerPosition;
}

const int EventManager::GetPlayerResources(const int& clientId, CheckpointList<ResourceComponent>::Node<ResourceComponent>* rit)
{
    const int playerPosition = FindPlayerByClientId(clientId, 0);
    int pos = 0;
    while (pos < playerPosition)
    {
        pos++;
        rit = m_resources->GetNextNode(&*rit);
    }
    return playerPosition;
}

