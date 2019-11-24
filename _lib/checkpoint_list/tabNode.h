#ifndef TAB_NODE_H__
#define TAB_NODE_H__

#include "checkpointNode.h"

template<class U>
struct TabNode
{
    TabNode()
    {
        checkpointNode = new CheckpointNode<U>(0);
        next = 0;
    }
    CheckpointNode<U>* checkpointNode;
    TabNode<U>* next;
};

#endif