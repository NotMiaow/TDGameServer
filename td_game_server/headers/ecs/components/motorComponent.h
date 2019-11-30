#ifndef	MOVEMENT_COMPONENT_H__
#define MOVEMENT_COMPONENT_H__

#include <queue>

#include "vector2.h"
#include "cst.h"

struct MotorComponent
{
	Behaviour behaviour;
	std::queue<Vector2> path;
    Vector2 normalizedTarget;
	float baseSpeed;
	float curSpeed;
};

#endif