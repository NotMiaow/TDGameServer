#ifndef PATH_H__
#define	PATH_H__

#include <vector>
#include "vector2.h"

struct Path
{
	Path() { }
	Path(const std::vector<Vector2>& path) : path(path), totalDistance(0), previousDistance(0) { }
	~Path() { }
	std::vector<Vector2> path;
	int totalDistance;
	int previousDistance;
};

#endif