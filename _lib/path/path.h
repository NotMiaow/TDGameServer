#ifndef PATH_H__
#define	PATH_H__

#include <queue>
#include "vector2.h"

struct Path
{
	Path() { }
	Path(const std::queue<Vector2>& path) : path(path), totalDistance(0), lastYDirection(0), lastXDirection(0) { }
	~Path() { }
	std::queue<Vector2> path;
	int totalDistance;
	int lastYDirection;
	int lastXDirection;
};

#endif