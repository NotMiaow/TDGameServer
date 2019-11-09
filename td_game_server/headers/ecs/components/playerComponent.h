#ifndef PLAYER_COMPONENT_H__
#define	PLAYER_COMPONENT_H__

#include "client.h"

struct PlayerComponent
{
	Client* client;
	bool alive;
	bool connected;
	bool ready;
	int towersOwned;
	int groupsOnGrid;
};

#endif
