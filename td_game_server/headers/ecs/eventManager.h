#ifndef EVENT_HANDLER_H__
#define EVENT_HANDLER_H__

#include <iostream>

#include "checkpointList.h"

class EventManager
{
public:
    EventManager() { }
    ~EventManager() { }
    void Seed(CheckpointList<int>& ints);
    void Terminate();
private:
    CheckpointList<int>* m_ints;
};

#endif
