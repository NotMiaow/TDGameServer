#ifndef MOVEMENT_SYSTEM_H__
#define MOVEMENT_SYSTEM_H__

#include <iostream>
#include <math.h>
#include <queue>

#include "basicLib.h"

#include "timeSystem.h"

#include "componentArray.h"
#include "vector2.h"
#include "path.h"
#include "motorComponent.h"
#include "transformComponent.h"

#include "definitions.h"

class MovementSystem
{
public:
	MovementSystem();
	MovementSystem(Motors& motors, Transforms& transforms);
	~MovementSystem();
	void Loop(const float& deltaTime);
private:
	void MoveMotor(const float& deltaTime, MotorComponent& motor, TransformComponent& transforms);
private:
	TimeSystem* m_timeSystem;

	Motors* m_motors;
	Transforms* m_transforms;
};

#endif