# TDGameServer

## Introduction
This is a project I (Louis-Étienne Parisien || github/Notmiaow) work on in my personnal time.
I aim to, at the very least, build it into a worthy portfolio project.

## Structure

### GameServer
![GameServer](td_game_server/headers/gameServer.h "GameServer") :
- Initiates data structures' structure (![CheckpointList<Component>](https://github.com/NotMiaow/td_game_server/blob/master/_lib/checkpoint_list/checkpointList.h "CheckpointList<Component>")).
- Creates a ![SharedQueue<Action>](https://github.com/NotMiaow/td_game_server/blob/master/_lib/queue/shared_queue.h "SharedQueue<Action>").
- Creates a ![NetworkManager](td_game_server/headers/networking/networkManager.h "NetworkManager") which operates on it's own and populates the ![SharedQueue<Action>](https://github.com/NotMiaow/td_game_server/blob/master/_lib/queue/shared_queue.h "SharedQueue<Action>").
- Creates a ![SharedQueue<Event>](https://github.com/NotMiaow/td_game_server/blob/master/_lib/queue/shared_queue.h "SharedQueue<Event>").
- Creates an ![ActionManager](td_game_server/headers/networking/actionManager.h "ActionManager") which empties the ![SharedQueue<Action>](https://github.com/NotMiaow/td_game_server/blob/master/_lib/queue/shared_queue.h "SharedQueue<Action>") once per frame and populates a std::queue<![Event](td_game_server/headers/networking/events/event.h "Event")>.
- Creates a ![EventManager](td_game_server/headers/ecs/eventManager.h "EventManager") that empties the std::queue<![Event](td_game_server/headers/networking/events/event.h "Event")> once per frame.


### NetworkManager
![NetworkManager](td_game_server/headers/networking/networkManager.h "NetworkManager") :
- Supports ONLY tcp.
- Waits for clients connection.
- Listens to clients.
- Parses received data into understandable format (![Action](td_game_server/headers/networking/actions/action.h "Actions")).

### ActionManager
![ActionManager](td_game_server/headers/networking/actionManager.h "ActionManager") :
- Authenticates client.
- Relays game related data to ![EventManager](td_game_server/headers/ecs/eventManager.h "EventManager").

### EventManager
![EventManager](td_game_server/headers/ecs/eventManager.h "EventManager") :
- Manages player Input :
  - Accepts and denies player requests.
  - Formats data to be sent to the player (![Event](td_game_server/headers/networking/events/event.h "Events")).
  - Relays client messaging to ![NetworkManager](td_game_server/headers/networking/networkManager.h "NetworkManager").
- Adds, removes and modifies groups of related components ("entities").

### ECS
![ECS](td_game_server/headers/ecs/ecs.h "ECS") :
- This system is not currently pure ECS as it does not benefit from cache locality. It is ECS like because data is stored as individual components and is modified within Systems and EventManager.
- Calls systems' individual Loops, passing a delta of time when required.

### Systems
![Systems](td_game_server/headers/ecs/systems "Systems") :
- Goes through required structure of components once per frame.
- Applies modification to components when applicable.
- Removes group of components ("Entities") when applicable.

## Compiling
- Requires Cmake
- On Linux :
  - Run "make" in terminal to compile the game server
  - Run ./_bin/debug to start the game server
- On Winwdows :
  - There is currently no windows support because of ![NetworkManager](td_game_server/headers/networking/networkManager.h "NetworkManager")'s implementation.
  - I am not sure wether or not this will be implemented later as I would use a Linux machine run this program if I ever had to..!
  - There will be Windows support in the Godot game client that goes with this game server (![GameClient](https://github.com/NotMiaow/td_game_client "td_game_client")).
