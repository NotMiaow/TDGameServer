#include "addSystem.h"

AddSystem::AddSystem(CheckpointList<int>& ints)
{
    m_ints = &ints;
}

void AddSystem::Loop()
{
    CheckpointList<int>::Node<int>* intN = m_ints->GetNodeHead();
    while(intN != 0)
    {
        Add(intN->data);
        intN = m_ints->GetNextNode(&*intN);
    }
}

void AddSystem::Add(const int& data)
{
    std::cout << (data + 1000) << std::endl;
}