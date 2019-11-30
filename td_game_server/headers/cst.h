#ifndef CST_H__
#define CST_H__

#include <string>

//Shared constants
const int MAX_CLIENTS = 4;

//Networking constants
const int BUFFER_LENGTH = 512;

//Engine constants
enum Behaviour { WaitingForPath, Move, Rage, Despawn };
const int MAX_GROUPS_ON_GRID = 9;
const int SPAWN_POSITION_Y = -4;
const int SPAWN_POSITION_X = 5;
const int SPAWN_SIZE = 4;
const int GRID_SIZE_X = 11;
const int GRID_SIZE_Y = 11;
const int DESPAWN_SIZE = 4;
const int STARTING_LIVES = 20;
const int STARTING_GOLD = 10000;    //temporarily high, helps debugging
const int STARTING_INCOME = 80;
const int POST_GAME_DURATION = 1; //Milliseconds

//ECS constants
enum PlayerCheckpoints { PLAYER_PLAYERS = 0, PLAYER_CHECKPOINT_SIZE };
enum BankCheckpoints { PLAYER_BANKS = 0, BANK_CHECKPOINT_SIZE };
enum TransformsCheckpoint { UNIT_GROUP_TRANSFORMS = 0, TOWER_TRANSFORMS, TRANSFORM_CHECKPOINT_SIZE };
enum MotorCheckpoints { UNIT_GROUP_MOTORS = 0, MOTOR_CHECKPOINT_SIZE };
enum OffenseCheckpointss { UNIT_GROUP_OFFENSES = 0, TOWER_OFFENSES, OFFENSE_CHECKPOINT_SIZE };

//Debugging constants
//These constants are used to fake that a clients were assigned to this game server
//Making this a test environment
const int PLAYER_IDS[MAX_CLIENTS] = { 45, 120, 645, 356 };
const std::string PLAYER_TOKENS[MAX_CLIENTS] = { "e2S5Ert6", "cy53rt0u", "3lc4vu12", "f4C7hagj" };
#endif