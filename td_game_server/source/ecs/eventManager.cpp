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

    //Seed players
    for(int i = 0; i < MAX_CLIENTS; i++)
    {
        PlayerComponent player;
        player.client = &clients[i];
        player.connected = false;
        player.ready = false;
        player.lives = STARTING_LIVES;
        m_players->InsertNode(player, i, PLAYER_PLAYERS);
    }

    //Seed resources
    for(int i = 0; i < MAX_CLIENTS; i++)
    {
        BankComponent bank;
        bank.gold = STARTING_GOLD;
        bank.income = STARTING_INCOME;
        m_banks->InsertNode(bank, i, PLAYER_BANKS);
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
    //    target2.x = 2;GetCheckpointHead
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

    //Get player
    CheckpointList<PlayerComponent>::Iterator playerIt(m_players->GetNodeHead(), 0);
    for(;!playerIt.End() && playerIt.Get()->client->id != event->clientId; playerIt++);
    PlayerComponent* player = playerIt.Get();

    player->connected = true;
    player->ready = false;
    m_networkManager->MessageClient(player->client->socketId, event->ToNetworkable());
}

void EventManager::DisconnectPlayer()
{

}

void EventManager::ReadyUpPlayer()
{
    ReadyUpEvent* event = dynamic_cast<ReadyUpEvent *>(m_event);


    //Get player and his position within components
    int playerPosition = 0;
    CheckpointList<PlayerComponent>::Iterator playerIt(m_players->GetNodeHead(), 0);
    for(;!playerIt.End() && playerIt.Get()->client->id != event->clientId; playerPosition++, playerIt++);
    PlayerComponent* player = playerIt.Get();

    //Ready up player
    player->ready = true;
    
    //Answer client
    event->playerPosition = playerPosition;
    event->players = m_players;        
    event->banks = m_banks;
    m_networkManager->MessageClient(player->client->socketId, event->ToNetworkable());
}

void EventManager::BuildTower()
{
    BuildTowerEvent* event = dynamic_cast<BuildTowerEvent*>(m_event);

    //Get player and his position within components
    int playerPosition = 0;
    CheckpointList<PlayerComponent>::Iterator playerIt(m_players->GetNodeHead(), 0);
    for(;!playerIt.End() && playerIt.Get()->client->id != event->clientId; playerPosition++, playerIt++);
    PlayerComponent* player = playerIt.Get();

    //Return if the player does not have ennough gold
    CheckpointList<BankComponent>::Iterator bankIt = m_banks->GetIterator(playerPosition, PLAYER_BANKS);
    if(bankIt.Get()->gold < TOWER_COSTS[0])
        return;
    BankComponent* bank = bankIt.Get();

    //Return if there is already a tower a the requested position
    CheckpointList<TransformComponent>::Iterator transformIt = m_transforms->GetIterator(playerPosition, TOWER_TRANSFORMS);
    for(;!transformIt.End(); transformIt++)
        if(transformIt.Get()->position.x == event->position.x && transformIt.Get()->position.y == event->position.y)
           return;

    //Get player's towers
    CheckpointList<OffenseComponent>::Iterator offenseIt = m_offenses->GetIterator(playerPosition, TOWER_OFFENSES);
    transformIt = m_transforms->GetIterator(playerPosition, TOWER_TRANSFORMS);

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
    //Insert components
    m_offenses->InsertNode(offense, playerPosition, TOWER_OFFENSES);
    m_transforms->InsertNode(transform, playerPosition, TOWER_TRANSFORMS);
    //Substract gold from bank
    bank->gold += -TOWER_COSTS[0];
    event->remainingGold = bank->gold;
    m_networkManager->MessageClient(player->client->socketId, event->ToNetworkable());
}