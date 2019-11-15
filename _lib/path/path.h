#ifndef PATH_H__
#define	PATH_H__

#include "vector2.h"
#include "queue.h"

struct Path
{
	Queue<Vector2> path;
	int length;
	int lastYDirection;
	int lastXDirection;
};

#endif