#include "eventManager.h"

EventManager::~EventManager()
{

}

void EventManager::Init(Client* clients, NetworkManager& networkManager, SharedQueue<Event*>& eventQueue, EntityCounter& entityCounter, Players& players, Banks& banks,
            Offenses& offenses, Motors& motors, Transforms& transforms)
{
    m_networkManager = &networkManager;
    m_eventQueue = &eventQueue;

    m_entityCounter = &entityCounter;
    m_players = &players;
    m_banks = &banks;
    m_offenses = &offenses;
    m_motors = &motors;
    m_transforms = &transforms;

    //Seed players
    for(int i = 0; i < MAX_CLIENTS; i++)
    {
        PlayerComponent player(&clients[i], false, false, STARTING_LIVES);
        BankComponent bank(STARTING_GOLD, STARTING_INCOME);
        
        int entityId = m_entityCounter->GetId();
        m_players->Add(player, entityId, GetCheckpoint(i, TPlayer, PLAYER_PLAYERS));
        m_banks->Add(bank, entityId, GetCheckpoint(i, TBank, PLAYER_BANKS));
    }

    //Seed resources
    for(int i = 0; i < MAX_CLIENTS; i++)
    {
    }
}

void EventManager::Loop()
{
    while (m_eventQueue->size())
    {
        m_event = m_eventQueue->front();
        if (m_event != 0) SwitchEvent();
        delete m_event;
        m_eventQueue->pop_front();
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
    //TEMPORARY
    case ESpawnUnitGroup:
        SpawnUnitGroup();
        break;
    case ENewPath:
        SendNewPath();
        break;
    case ERage:
        Rage();
        break;
    case EBuildTower:
        BuildTower();
        break;
    case ESellTower:
        SellTower();
        break;
    case ESendUnitGroup:
        break;
    }
}

void EventManager::ConnectPlayer()
{
    ConnectEvent* event = dynamic_cast<ConnectEvent *>(m_event);

    //Get player
    int socketId;
    for(PlayerIterator playerIt = m_players->GetIterator(); !playerIt.End(); playerIt++)
    {
        PlayerComponent* player = playerIt.GetData();
        if(player->client->id == event->clientId)
        {
            player->connected = true;
            player->ready = false;
            socketId = player->client->socketId;
            break;
        }
    }

    //Answer client
    m_networkManager->MessageClient(socketId, event->ToNetworkable());
}

void EventManager::DisconnectPlayer()
{

}

void EventManager::ReadyUpPlayer()
{
    ReadyUpEvent* event = dynamic_cast<ReadyUpEvent *>(m_event);

    int socketId;
    for(PlayerIterator playerIt = m_players->GetIterator(); !playerIt.End(); playerIt++)
    {
        Players::Entry* player = playerIt.GetEntry();
        if(player->data.client->id == event->clientId)
        {
            player->data.ready = true;
            event->playerId = player->entityId;
            event->clientId = player->data.client->id;
            socketId = player->data.client->socketId;
        }
        event->entityIds->push_back(player->entityId);
    }

    event->banks = m_banks;
    event->players = m_players;

    std::cout << event->ToNetworkable() << std::endl;

    m_networkManager->MessageClient(socketId, event->ToNetworkable());
}

void EventManager::SpawnUnitGroup()
{
    SpawnUnitGroupEvent* event = dynamic_cast<SpawnUnitGroupEvent*>(m_event);

    int socketId;
    int playerPosition;
    for(PlayerIterator playerIt = m_players->GetIterator(); !playerIt.End(); playerIt++)
    {
        Players::Row row = playerIt.GetRow();
        if(row.data->client->id == event->clientId)
        {
            playerPosition = row.index;
            socketId = row.data->client->socketId;
            event->playerId = row.entityId;
            break;
        }
    }

    //Create unit group
    MotorComponent motor(2);
    TransformComponent transform(Vector2(SPAWN_POSITION_X, SPAWN_POSITION_Y));
    OffenseComponent offense(1, 1);

    event->entityId = m_entityCounter->GetId();
    m_motors->Add(motor, event->entityId, GetCheckpoint(playerPosition, TMotor, UNIT_GROUP_MOTORS));
    m_transforms->Add(transform, event->entityId, GetCheckpoint(playerPosition, TTransform, UNIT_GROUP_TRANSFORMS));
    m_offenses->Add(offense, event->entityId, GetCheckpoint(playerPosition, TOffense, UNIT_GROUP_OFFENSES));

    std::cout << event->ToNetworkable() << std::endl;
    m_networkManager->MessageClient(socketId, event->ToNetworkable());
}

void EventManager::SendNewPath()
{
    NewPathEvent* event = dynamic_cast<NewPathEvent*>(m_event);
    
    const int playerPosition = m_motors->GetCheckpointPosition(event->entityId);

    int socketId;
    Players::Row player = m_players->GetIterator(GetCheckpoint(playerPosition, TMotor, PLAYER_PLAYERS)).GetRow();
    event->playerId = player.entityId;
    socketId = player.data->client->socketId;

    std::cout << event->ToNetworkable() << std::endl;
    m_networkManager->MessageClient(socketId, event->ToNetworkable());    
}

void EventManager::Rage()
{
    RageEvent* event = dynamic_cast<RageEvent*>(m_event);

    int socketId;
    for(PlayerIterator playerIt = m_players->GetIterator(); !playerIt.End(); playerIt++)
    {
        PlayerComponent* player = playerIt.GetData();
        if(player->client->id == event->clientId)
        {
            socketId = player->client->socketId;
            break;
        }
    }
    
    std::cout << event->ToNetworkable() << std::endl;
    m_networkManager->MessageClient(socketId, event->ToNetworkable());    
}

void EventManager::BuildTower()
{
    BuildTowerEvent* event = dynamic_cast<BuildTowerEvent*>(m_event);

    //Get player and his position within components
    int socketId;
    int playerPosition;
    for(PlayerIterator playerIt = m_players->GetIterator(); !playerIt.End(); playerIt++)
    {
        Players::Row player = playerIt.GetRow();
        if(player.data->client->id == event->clientId)
        {
            playerPosition = player.index;
            socketId = player.data->client->socketId;
            event->playerId = player.entityId;
            break;
        }
    }

    //Return if the player does not have ennough gold
    Banks::Row bank = m_banks->GetById(event->playerId);
    if(bank.data->gold < TOWER_COSTS[event->towerType])
        return;

    //Return if there is already a tower a the requested position
    for(TransformIterator transformIt = m_transforms->GetIterator(GetCheckpoint(playerPosition, TTransform, UNIT_GROUP_TRANSFORMS));
        !transformIt.End(); transformIt++)
        if(transformIt.GetData()->position.x == event->position.x && transformIt.GetData()->position.y == event->position.y)
           return;

    //Create tower entry
    OffenseComponent offense(1, 1);
    TransformComponent transform(event->position);

    event->entityId = m_entityCounter->GetId();
    m_offenses->Add(offense, event->entityId, GetCheckpoint(playerPosition, TOffense, TOWER_OFFENSES));
    m_transforms->Add(transform, event->entityId, GetCheckpoint(playerPosition, TTransform, TOWER_TRANSFORMS));

    //Substract gold from bank
    bank.data->gold += -TOWER_COSTS[event->towerType];

    //Set motors' behaviour, 
    //whose paths are being blocked by the newly built tower
    //to WaitingForPath
    for(MotorIterator motorIt = m_motors->GetIterator(GetCheckpoint(playerPosition, TMotor, UNIT_GROUP_MOTORS));
        !motorIt.End(); motorIt++)
        motorIt.GetData()->behaviour = WaitingForPath;

    //Answer client
    event->remainingGold = bank.data->gold;
    m_networkManager->MessageClient(socketId, event->ToNetworkable());
}

void EventManager::SellTower()
{
    SellTowerEvent* event = dynamic_cast<SellTowerEvent*>(m_event);

    int socketId;
    int playerPosition;
    for(PlayerIterator playerIt = m_players->GetIterator(); !playerIt.End(); playerIt++)
    {
        Players::Row player = playerIt.GetRow();
        if(player.data->client->id == event->clientId)
        {
            playerPosition = player.index;
            socketId = player.data->client->socketId;
            event->playerId = player.entityId;
            break;
        }
    }

    //If tower does not exist
    if(m_transforms->GetById(event->entityId).entityId == -1)
    {
        std::cout << "tower does not exist, *****resynchronise client*****" << std::endl;
        return;
    }

    //Remove the tower
    m_transforms->Remove(event->entityId);
    m_offenses->Remove(event->entityId);

    //Give gold back
    Banks::Row bank = m_banks->GetById(event->playerId);
    bank.data->gold += TOWER_COSTS[0];

    //Set all player's motors' behaviour to WaitingForPath
    for(MotorIterator motorIt = m_motors->GetIterator(GetCheckpoint(playerPosition, TMotor, UNIT_GROUP_MOTORS)); !motorIt.End(); motorIt++)
        motorIt.GetData()->behaviour = WaitingForPath;

    //AnswerClient
    event->remainingGold = bank.data->gold;
    std::cout << event->ToNetworkable() << std::endl;
    m_networkManager->MessageClient(socketId, event->ToNetworkable());
}