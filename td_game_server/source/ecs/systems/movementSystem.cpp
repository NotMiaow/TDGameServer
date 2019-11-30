#include "movementSystem.h"

MovementSystem::MovementSystem()
{
}

MovementSystem::MovementSystem(CheckpointList<MotorComponent>& motors, CheckpointList<TransformComponent>& transforms)
{
	//Components
	m_motors = &motors;
	m_transforms = &transforms;
}

MovementSystem::~MovementSystem()
{

}

void MovementSystem::Loop(const float& deltaTime)
{
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		CheckpointList<MotorComponent>::Iterator motorIt = m_motors->GetIterator(i, UNIT_GROUP_MOTORS);
		CheckpointList<TransformComponent>::Iterator transformIt = m_transforms->GetIterator(i, UNIT_GROUP_TRANSFORMS);
		for(; !motorIt.End(); motorIt++, transformIt++)
			if(!motorIt.Get()->path.empty())
				MoveMotor(deltaTime, *motorIt.Get(), *transformIt.Get());
	}
}

void MovementSystem::MoveMotor(const float& deltaTime, MotorComponent& motor, TransformComponent& transform)
{
	//Translate motor
	transform.position.x += motor.normalizedTarget.x * motor.curSpeed * deltaTime;
	transform.position.y += motor.normalizedTarget.y * motor.curSpeed * deltaTime;

	Vector2 targetPosition = motor.path.front();
	std::cout << transform.position.y << ":" << transform.position.x << std::endl;

	//If passed target
	if ((motor.normalizedTarget.x > 0 && transform.position.x >= targetPosition.x ||
		motor.normalizedTarget.x < 0 && transform.position.x <= targetPosition.x) || 
		(motor.normalizedTarget.y > 0 && transform.position.y >= targetPosition.y ||
		motor.normalizedTarget.y < 0 && transform.position.y <= targetPosition.y))
	{
		//Set position as target's position
		transform.position.x = targetPosition.x;
		transform.position.y = targetPosition.y;
		
		//Remove target from motor's path
		motor.path.pop();

		//Update normalised target
		if(!motor.path.empty())
		{
			targetPosition = motor.path.front();
			float distanceX = targetPosition.x - transform.position.x;
			float distanceY = targetPosition.y - transform.position.y;
			float distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));
			motor.normalizedTarget.x = distanceX / distance;
			motor.normalizedTarget.y = distanceY / distance;
		}
		else
		{
			motor.normalizedTarget.x = 0;
			motor.normalizedTarget.y = 0;
		}
	}
}