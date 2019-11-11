#include "movementSystem.h"

MovementSystem::MovementSystem()
{
}

MovementSystem::MovementSystem(TimeSystem& timeSystem, CheckpointList<MotorComponent>& motors, CheckpointList<TransformComponent>& transforms)
{
	//Time
	m_timeSystem = &timeSystem;

	//Components
	m_motors = &motors;
	m_transforms = &transforms;
}

MovementSystem::~MovementSystem()
{

}

void MovementSystem::Loop()
{
	CheckpointList<MotorComponent>::Node<MotorComponent>* mNode = m_motors->GetNodeHead();
	CheckpointList<TransformComponent>::Node<TransformComponent>* tNode = m_transforms->GetNodeHead();
	while (mNode != NULL)
	{
		SwitchBehaviour(mNode->data, tNode->data);
		mNode = m_motors->GetNextNode(&*mNode);
		tNode = m_transforms->GetNextNode(&*tNode);
	}
}

void MovementSystem::SwitchBehaviour(MotorComponent& motor, TransformComponent& transform)
{
	if (motor.path.GetSize() > 0)
	{
		switch (motor.behaviour)
		{
		case Move:
			MoveMotor(motor, transform);
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

void MovementSystem::MoveMotor(MotorComponent& motor, TransformComponent& transform)
{
	//Translate motor
	transform.position.x += transform.normalizedTarget.x * motor.curSpeed * (float)m_timeSystem->DeltaTime();
	transform.position.y += transform.normalizedTarget.y * motor.curSpeed * (float)m_timeSystem->DeltaTime();

	//If passed target
	if (transform.normalizedTarget.x > 0 && transform.position.x >= motor.path.GetHead()->data.x ||
		transform.normalizedTarget.x < 0 && transform.position.x <= motor.path.GetHead()->data.x)
	{
		//Set position as target's position
		transform.position.x = motor.path.GetHead()->data.x;
		transform.position.y = motor.path.GetHead()->data.y;
		//Remove target from motor's path
		motor.path.RemoveHead();

		//Update normalised target
		if(motor.path.GetSize() > 0)
		{
			float distanceX = motor.path.GetHead()->data.x - transform.position.x;
			float distanceY = motor.path.GetHead()->data.y - transform.position.y;
			float distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));
			transform.normalizedTarget.x = distanceX / distance;
			transform.normalizedTarget.y = distanceY / distance;
		}
	}
}