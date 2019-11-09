#ifndef PATH_H__
#define	PATH_H__

#include "node.h"
#include "chain.h"
#include "position.h"

struct Path
{
	Chain<Position>* path;
	int length;
	int lastYDirection;
	int lastXDirection;
};

#endif