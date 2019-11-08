#ifndef ADD_SYSTEM_H__
#define ADD_SYSTEM_H__

#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

#include "cst.h"
#include "checkpointList.h"

class AddSystem
{
public:
    AddSystem() { }
    ~AddSystem() { }
    AddSystem(CheckpointList<int>& ints);
    void ProcessData();
    void Add(const int& data);
private:
    CheckpointList<int>* m_ints;
};

#endif