#ifndef DATA_NODE_H__
#define DATA_NODE_H__

template<class U>
struct DataNode
{
	DataNode(const U& data, DataNode<U>* next) : data(data), next(next) { }
	U data;
	DataNode<U>* next;
};


#endif