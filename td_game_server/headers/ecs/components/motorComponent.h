#ifndef	MOVEMENT_COMPONENT_H__
#define MOVEMENT_COMPONENT_H__

#include "vector2.h"
#include "chain.h"
#include "cst.h"

struct MotorComponent
{
	Behaviour behaviour;
	Chain<Vector2> path;
	float baseSpeed;
	float curSpeed;
};

#endif