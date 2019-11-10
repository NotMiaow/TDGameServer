#ifndef	MOVEMENT_COMPONENT_H__
#define MOVEMENT_COMPONENT_H__

#include "vector2.h"
#include "chain.h"

struct MotorComponent
{
	Chain<Vector2>* path;
	float baseSpeed;
	float curSpeed;
};

#endif