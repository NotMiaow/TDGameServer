#ifndef NODE_H__
#define NODE_H__

template <class T>
struct Node
{
	T component;
	Node<T>* next;
};

#endif
