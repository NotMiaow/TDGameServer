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
	CheckpointList<MotorComponent>::Node<MotorComponent>* mNode = m_motors->GetNodeHead();
	CheckpointList<TransformComponent>::Node<TransformComponent>* tNode = m_transforms->GetNodeHead();
	while (mNode != NULL)
	{
		SwitchBehaviour(deltaTime, mNode->data, tNode->data);
		mNode = m_motors->GetNextNode(&*mNode);
		tNode = m_transforms->GetNextNode(&*tNode);
	}
}

void MovementSystem::SwitchBehaviour(const float& deltaTime, MotorComponent& motor, TransformComponent& transform)
{
	if (motor.path.GetSize())
	{
		switch (motor.behaviour)
		{
		case Move:
			MoveMotor(deltaTime, motor, transform);
			break;
		case Rage:
			break;
		case Despawn:
			break;
		default:
			break;
		}
	}
}

void MovementSystem::MoveMotor(const float& deltaTime, MotorComponent& motor, TransformComponent& transform)
{
	//Translate motor
	transform.position.x += transform.normalizedTarget.x * motor.curSpeed * deltaTime;
	transform.position.y += transform.normalizedTarget.y * motor.curSpeed * deltaTime;

	Vector2 targetPosition = motor.path.Front();
	std::cout << transform.position.y << ":" << transform.position.x << std::endl;

	//If passed target
	if (transform.normalizedTarget.x > 0 && transform.position.x >= targetPosition.x ||
		transform.normalizedTarget.x < 0 && transform.position.x <= targetPosition.x)
	{
		//Set position as target's position
		transform.position.x = targetPosition.x;
		transform.position.y = targetPosition.y;
		
		//Remove target from motor's path
		targetPosition = motor.path.Pop();

		//Update normalised target
		if(motor.path.GetSize())
		{
			targetPosition = motor.path.Front();
			float distanceX = targetPosition.x - transform.position.x;
			float distanceY = targetPosition.y - transform.position.y;
			float distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));
			transform.normalizedTarget.x = distanceX / distance;
			transform.normalizedTarget.y = distanceY / distance;
		}
	}
}