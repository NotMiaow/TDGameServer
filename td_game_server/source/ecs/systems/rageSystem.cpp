#include "rageSystem.h"

RageSystem::RageSystem(SharedQueue<Event*>& eventQueue, Motors& motors, Transforms& transforms)
{
    m_eventQueue = &eventQueue;
    m_motors = &motors;
    m_transforms = &transforms;
}

void RageSystem::Loop()
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
            Motors::Row motor = std::get<0>(j).GetRow();
            TransformComponent* transform = std::get<1>(j).GetData();

            if(motor.data->behaviour == Rage)
                AssignTarget(i, motor.index, *motor.data, *transform);
        }
    }
}

void RageSystem::AssignTarget(const int& playerIndex, const int& motorPosition, MotorComponent& motor, const TransformComponent& transform)
{
//    CheckpointList<TransformComponent>::Iterator transformIt = m_transforms->GetIterator(playerIndex, TOWER_TRANSFORMS);
    motor.normalizedTarget.x = 0;
    motor.normalizedTarget.y = 0;
}