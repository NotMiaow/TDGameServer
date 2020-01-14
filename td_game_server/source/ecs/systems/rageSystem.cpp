#include "rageSystem.h"

RageSystem::RageSystem(SharedQueue<Event*>& eventQueue, CheckpointList<MotorComponent>& motors, CheckpointList<TransformComponent>& transforms)
{
    m_eventQueue = &eventQueue;
    m_motors = &motors;
    m_transforms = &transforms;
}

void RageSystem::Loop()
{
    for(int i = 0; i < MAX_CLIENTS; i++)
    {
        CheckpointList<MotorComponent>::Iterator motorIt = m_motors->GetIterator(i, UNIT_GROUP_MOTORS);
        CheckpointList<TransformComponent>::Iterator transformIt = m_transforms->GetIterator(i, UNIT_GROUP_TRANSFORMS);
        for(int j = 0;!motorIt.End(); j++, motorIt++, transformIt++)
            if(motorIt.Get()->behaviour == WaitingForPath)
                AssignTarget(i, j, *motorIt.Get(), *transformIt.Get());
    }
}

void RageSystem::AssignTarget(const int& playerIndex, const int& motorPosition, MotorComponent& motor, const TransformComponent& transform)
{
    CheckpointList<TransformComponent>::Iterator transformIt = m_transforms->GetIterator(playerIndex, TOWER_TRANSFORMS);
    
}