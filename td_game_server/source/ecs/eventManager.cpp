#include "eventManager.h"

EventManager::~EventManager()
{

}

void EventManager::Init(Client *clients, NetworkManager& networkManager, SharedQueue<Event *> &eventQueue, CheckpointList<PlayerComponent> &players,
                        CheckpointList<BankComponent> &banks, CheckpointList<OffenseComponent>& offenses, CheckpointList<MotorComponent> &motors,
                        CheckpointList<TransformComponent> &transforms)
{
    m_networkManager = &networkManager;
    m_eventQueue = &eventQueue;

    m_players = &players;
    m_banks = &banks;
    m_offenses = &offenses;
    m_motors = &motors;
    m_transforms = &transforms;

    int cnt = 0;
    {
        //Seed players
        TabNode<PlayerComponent> *tabIt = m_players->GetTabHead();
        while(tabIt)
        {
            PlayerComponent player;
            player.client = &clients[cnt];
            player.connected = false;
            player.ready = false;
            player.lives = STARTING_LIVES;
            m_players->InsertNode(player, tabIt->checkpointNode);
            tabIt = m_players->GetNextTab(&*tabIt);
            cnt++;
        }
    }
    {
        //Seed resources
        TabNode<BankComponent> *tabIt = m_banks->GetTabHead();
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            BankComponent bank;
            bank.gold = STARTING_GOLD;
            bank.income = STARTING_INCOME;
            m_banks->InsertNode(bank, tabIt->checkpointNode);
            tabIt = m_banks->GetNextTab(&*tabIt);
        }
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

void EventManager::Loop()
{
    while (m_eventQueue->GetSize())
    {
        m_event = m_eventQueue->Pop();
        if (m_event != 0) SwitchEvent();
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
        DisconnectPlayer();
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
        BuildTower();
        break;
    case ESellTower:
        break;
    case ESendUnitGroup:
        break;
    }
}

void EventManager::ConnectPlayer()
{
    ConnectEvent* event = dynamic_cast<ConnectEvent *>(m_event);
    DataNode<PlayerComponent> *pit = m_players->GetNodeHead();
    FindPlayerByClientId(event->clientId, pit);

    pit->data.connected = true;
    pit->data.ready = false;
    m_networkManager->MessageClient(pit->data.client->socketId, event->ToNetworkable());
}

void EventManager::DisconnectPlayer()
{

}

void EventManager::ReadyUpPlayer()
{
    ReadyUpEvent* event = dynamic_cast<ReadyUpEvent *>(m_event);
    DataNode<PlayerComponent> *pit = m_players->GetNodeHead();
    const int playerPosition = FindPlayerByClientId(event->clientId, pit);

    pit->data.ready = true;
    event->playerPosition = playerPosition;
    event->players = m_players;        
    event->banks = m_banks;
    m_networkManager->MessageClient(pit->data.client->socketId, event->ToNetworkable());
    return;
}

void EventManager::BuildTower()
{
    BuildTowerEvent* event = dynamic_cast<BuildTowerEvent*>(m_event);

    //Get player
    DataNode<PlayerComponent>* playerIt = m_players->GetNodeHead();
    const int playerPosition = FindPlayerByClientId(event->clientId, playerIt);

    //Return if the player does not have ennough gold
    DataNode<BankComponent>* bankIt = m_banks->GetNodeHead();
    for(int i = 0; i < playerPosition; i++, bankIt = m_banks->GetNextNode(&*bankIt));
    if(bankIt->data.gold < TOWER_COSTS[0])
        return;

    //Get player's towers
    CheckpointNode<OffenseComponent>* offenseIt = m_offenses->GetTabHead()->checkpointNode;
    CheckpointNode<TransformComponent>* transformIt = m_transforms->GetTabHead()->checkpointNode;
    GetPlayerTowers(playerPosition, offenseIt, transformIt);

    //Return if there is already a tower a the requested position
    DataNode<TransformComponent>* nodeIt;
    for (nodeIt = transformIt->node; nodeIt && nodeIt != transformIt->next->node; nodeIt = m_transforms->GetNextNode(&*nodeIt))
        if(nodeIt->data.position.x == event->position.x && nodeIt->data.position.y == event->position.y)
            return;

    //Create tower's OffenseComponent
    OffenseComponent offense;
    offense.baseAttackRate = 1;
    offense.baseDamage = 1;
    offense.curAttackRate = offense.baseAttackRate;
    offense.curDamage = offense.baseDamage;
    //Create tower's TransformComponent
    TransformComponent transform;
    transform.position.x = event->position.x;
    transform.position.y = event->position.y;

    m_offenses->InsertNode(offense, offenseIt);
    m_transforms->InsertNode(transform, transformIt);

    //Substract gold from bank
    bankIt->data.gold += -TOWER_COSTS[0];
    event->remainingGold = bankIt->data.gold;
    m_networkManager->MessageClient(playerIt->data.client->socketId, event->ToNetworkable());
}

const int EventManager::FindPlayerByClientId(const int &clientId, DataNode<PlayerComponent> * pit)
{
    int playerPosition = 0;
    if(pit == 0) pit = m_players->GetNodeHead();
    while (pit->data.client->id != clientId)
    {
        playerPosition++;
        pit = m_players->GetNextNode(&*pit);
    }
    return playerPosition;
}

void EventManager::GetPlayerTowers(const int& playerPosition, CheckpointNode<OffenseComponent>* offenseIt, CheckpointNode<TransformComponent>* transformIt)
{
    TabNode<OffenseComponent>* offenseTabIt = m_offenses->GetTabHead();
    TabNode<TransformComponent>* transformTabIt = m_transforms->GetTabHead();
    int cnt = 0;
    
    //Get tabs
    while (cnt++ < playerPosition)
    {
        offenseTabIt = m_offenses->GetNextTab(&*offenseTabIt);
        transformTabIt = m_transforms->GetNextTab(&*transformTabIt);
    }

    //Get offense checkpoint
    cnt = 0; offenseIt = offenseTabIt->checkpointNode;
    while (cnt++ < O_TOWER) offenseIt = m_offenses->GetNextCheckpoint(offenseIt);
    //Get transform checkpoint
    cnt = 0; transformIt = transformTabIt->checkpointNode;
    while (cnt++ < T_TOWER) transformIt = m_transforms->GetNextCheckpoint(transformIt);
}