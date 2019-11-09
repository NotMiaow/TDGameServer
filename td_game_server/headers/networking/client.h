#ifndef CLIENT_H__
#define CLIENT_H__

#include <string>

struct Client
{
	int id;
	std::string token;
	int socketId;
};
#endif