#ifndef RAGE_SYSTEM_H__
#define RAGE_SYSTEM_H__

#include <algorithm>
#include <vector>
#include <math.h>

//Events
#include "event.h"
#include "shared_queue.h"

//Path
#include "vector2.h"
#include "path.h"

#include "checkpointList.h"
//Components
#include "motorComponent.h"
#include "transformComponent.h"

//Misc
#include "cst.h"

class RageSystem
{
public:
    RageSystem() { }
    RageSystem(SharedQueue<Event*>& eventQueue, CheckpointList<MotorComponent>& motors, CheckpointList<TransformComponent>& transforms);
    ~RageSystem() { }
    void Loop();
private:
    void AssignTarget(const int& playerIndex, const int& motorPosition, MotorComponent& motor, const TransformComponent& transform);
private:
    SharedQueue<Event*>* m_eventQueue;
    CheckpointList<MotorComponent>* m_motors;
    CheckpointList<TransformComponent>* m_transforms;
};

#endif