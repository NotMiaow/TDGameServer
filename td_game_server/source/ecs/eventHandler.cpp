#include "eventHandler.h"

void EventHandler::Seed(CheckpointList<int>& ints)
{
    m_ints = &ints;
    //Data sample
    const int NUMBERS_COUNT = 20;
    int numbers[NUMBERS_COUNT] = { 164,648,755,826,177,638,219,795,372,674,985,846,773,988,751,462,547,784,642,154 };
    
    //Data seeding
    CheckpointList<int>::Node<int>* intsN = m_ints->GetNodeHead();
    CheckpointList<int>::TabNode<int>* intsT = m_ints->InsertAfterTab(m_ints->GetTabHead());
    CheckpointList<int>::CheckpointNode<int>* intsCP = intsT->checkpointNode;
    for(int i = 0; i < NUMBERS_COUNT; i++)
    {
        intsN = m_ints->InsertAfterNode(numbers[i], intsN);
        if(i % 5 == 0)
            intsCP = m_ints->InsertAfterCheckpoint(intsN, intsCP, m_ints->GetTabHead());
    }
}