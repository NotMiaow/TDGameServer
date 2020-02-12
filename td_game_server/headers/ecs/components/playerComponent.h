#ifndef PLAYER_COMPONENT_H__
#define	PLAYER_COMPONENT_H__

#include "client.h"

struct PlayerComponent
{
	PlayerComponent() : client(0), connected(false), ready(false), lives(0) { }
	PlayerComponent(Client* client, const bool& connected, const bool& ready, const int& lives) : 
		client(client), connected(connected), ready(ready), lives(lives) { }
	Client* client;
	bool connected;
	bool ready;
	int lives;
};

#endif
