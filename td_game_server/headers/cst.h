#ifndef CST_H__
#define CST_H__

#include <string>

//Shared constants
static const int MAX_CLIENTS = 4;

//Networking constants
static const int PROCESSING_THREADS_COUNT = 8;
static const int TOKEN_LENGTH = 8;

//Engine constants
enum State { MoveInSpawn, MoveInPath, MoveInDespawn, Rage, Despawn };
const int MAX_GROUPS_ON_GRID = 9;
const int SPAWN_POSITION_Y = -4;
const int SPAWN_POSITION_X = 5;
const int SPAWN_SIZE = 4;
const int GRID_SIZE_X = 11;
const int GRID_SIZE_Y = 11;
const int DESPAWN_SIZE = 4;
const int STARTING_GOLD = 10000;
const int POST_GAME_DURATION = 1; //Milliseconds

//Debugging constants
//These constants are used to fake that a clients were assigned to this game server
//Making this a test environment
const int PLAYER_IDS[MAX_CLIENTS] = { 45, 120, 645, 356 };
const std::string PLAYER_TOKENS[MAX_CLIENTS] = { "e2S5Ert6", "cy53rt0u", "3lc4vu12", "f4C7hagj" };
#endif