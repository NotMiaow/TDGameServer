#ifndef	MOVEMENT_COMPONENT_H__
#define MOVEMENT_COMPONENT_H__

#include "vector2.h"
#include "queue.h"
#include "cst.h"

struct MotorComponent
{
	Behaviour behaviour;
	Queue<Vector2> path;
	float baseSpeed;
	float curSpeed;
};

#endif