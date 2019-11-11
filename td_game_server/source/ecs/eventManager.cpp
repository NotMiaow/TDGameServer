#include "eventManager.h"

EventManager::EventManager(CheckpointList<MotorComponent>& motors, CheckpointList<TransformComponent>& transforms)
{
    m_motors = &motors;
    m_transforms = &transforms;


    //Create Motor
    MotorComponent motor;
    Chain<Vector2> path;
    Vector2 target1;
    Vector2 target2;
    Vector2 target3;
    Vector2 target4;
    target1.x = 8;
    target1.y = 3;
    target2.x = 2;
    target2.y = 6;
    target3.x = 10;
    target3.y = 8;
    target4.x = 2;
    target4.y = GRID_SIZE_Y + DESPAWN_SIZE;
    motor.path.AddHead(target4);
    motor.path.AddHead(target3);
    motor.path.AddHead(target2);
    motor.path.AddHead(target1);
    motor.behaviour = Move;
    motor.baseSpeed = 2;
    motor.curSpeed = 2;
    m_motors->InsertAfterNode(motor, m_motors->GetNodeHead());

    //Create Transform
    TransformComponent transform;
    transform.position.x = SPAWN_POSITION_X;
    transform.position.y = SPAWN_POSITION_Y;
    //Update normalised target
	float distanceX = motor.path.GetHead()->data.x - transform.position.x;
	float distanceY = motor.path.GetHead()->data.y - transform.position.y;
	float distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));
	transform.normalizedTarget.x = distanceX / distance;
	transform.normalizedTarget.y = distanceY / distance;
    m_transforms->InsertAfterNode(transform, m_transforms->GetNodeHead());
}

EventManager::~EventManager()
{
    
}