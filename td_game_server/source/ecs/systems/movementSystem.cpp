#include "movementSystem.h"

MovementSystem::MovementSystem()
{
}

MovementSystem::MovementSystem(Motors& motors, Transforms& transforms)
{
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
        for(std::tuple<MotorIterator, TransformIterator> j = std::make_tuple 
            (
                m_motors->GetIterator(GetCheckpoint(i, TMotor, UNIT_GROUP_MOTORS)),
                m_transforms->GetIterator(GetCheckpoint(i, TTransform, UNIT_GROUP_TRANSFORMS))
            );
            !std::get<0>(j).End(); std::get<0>(j)++, std::get<1>(j)++)
        {
			MotorComponent* motor = std::get<0>(j).GetData();
			TransformComponent* transform = std::get<1>(j).GetData();
			if(!motor->path.empty())
				MoveMotor(deltaTime, *motor, *transform);
        }
	}
}

void MovementSystem::MoveMotor(const float& deltaTime, MotorComponent& motor, TransformComponent& transform)
{
	if(motor.normalizedTarget.x == 0 && motor.normalizedTarget.y == 0)
		return;

	//Translate motor
	transform.position.x += motor.normalizedTarget.x * motor.curSpeed * deltaTime;
	transform.position.y += motor.normalizedTarget.y * motor.curSpeed * deltaTime;

	Vector2 targetPosition = motor.path.front();

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
		motor.path.pop_front();

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