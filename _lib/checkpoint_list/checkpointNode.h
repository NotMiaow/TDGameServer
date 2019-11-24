#ifndef CHECKPOINT_NODE_H__
#define CHECKPOINT_NODE_H__

#include "dataNode.h"

template<class U>
struct CheckpointNode
{
    CheckpointNode(CheckpointNode<U>* nextCheckpoint)
    {
        node = 0;
        next = nextCheckpoint;
    }
    DataNode<U>* node;
    CheckpointNode<U>* next;
};

#endif