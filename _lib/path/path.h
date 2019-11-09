#ifndef PATH_H__
#define	PATH_H__

#include "chain.h"
#include "vector2.h"

struct Path
{
	Chain<Vector2>* path;
	int length;
	int lastYDirection;
	int lastXDirection;
};

#endif