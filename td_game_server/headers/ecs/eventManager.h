#ifndef EVENT_HANDLER_H__
#define EVENT_HANDLER_H__

#include <iostream>
#include <math.h>

#include "motorComponent.h"
#include "transformComponent.h"

#include "checkpointList.h"

class EventManager
{
public:
    EventManager() { }
    EventManager(CheckpointList<MotorComponent>& motors, CheckpointList<TransformComponent>& transforms);
    ~EventManager();
private:

private:
   	CheckpointList<MotorComponent>* m_motors;
	CheckpointList<TransformComponent>* m_transforms;
};

#endif
