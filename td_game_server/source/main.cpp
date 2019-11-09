#include <iostream>

#include "cst.h"
#include "client.h"
#include "gameServer.h"

int main()
{
	Client clients[MAX_CLIENTS];
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		clients[i].socketId = -1;
		clients[i].id = PLAYER_IDS[i];
		clients[i].token = PLAYER_TOKENS[i];
	}
	GameServer gameServer(27016, clients);
	std::cin.get();
}
