#ifndef MOVEMENT_SYSTEM_H__
#define MOVEMENT_SYSTEM_H__

#include <iostream>
#include <math.h>

#include "basicLib.h"

#include "timeSystem.h"

#include "checkpointList.h"
#include "vector2.h"
#include "path.h"
#include "queue.h"
#include "motorComponent.h"
#include "transformComponent.h"

class MovementSystem
{
public:
	MovementSystem();
	MovementSystem(CheckpointList<MotorComponent>& motors, CheckpointList<TransformComponent>& transforms);
	~MovementSystem();
	void Loop(const float& deltaTime);
private:
	void SwitchBehaviour(const float& deltaTime, MotorComponent& motor, TransformComponent& transforms);
	void MoveMotor(const float& deltaTime, MotorComponent& motor, TransformComponent& transforms);
private:
	TimeSystem* m_timeSystem;

	CheckpointList<MotorComponent>* m_motors;
	CheckpointList<TransformComponent>* m_transforms;
};

#endif