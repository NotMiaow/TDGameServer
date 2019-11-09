#ifndef CHAIN_H__
#define CHAIN_H__

#include <iostream>
#include "node.h"

template <class T>
class Chain
{
public:
	Chain();
	Chain(const Chain& source);
	~Chain();
	void AddHead(T& component);
	void AddTail(T& component);
	void InsertBefore(T& component, Node<T>* node);
	void InsertAt(T& component, int pos);
	void RemoveHead();
	void RemoveTail();
	void Remove(Node<T>* node);
	Node<T>* GetHead() const;
	Node<T>* GetTail() const;
	Node<T>* GetNext(Node<T> current) const;
	int GetSize() const;
private:
	int m_size;
	Node<T>* m_head;
};

template<class T>
inline Chain<T>::Chain()
{
	m_head = NULL;
	m_size = 0;
}

template<class T>
inline Chain<T>::Chain(const Chain& source) : m_head(NULL)
{
	Node<T>* node = source.GetHead();
	while (node != NULL)
	{
		AddTail(node->component);
		node = source.GetNext(*node);
	}
	m_size = source.m_size;
}

template<class T>
inline Chain<T>::~Chain()
{
	while (m_head != NULL)
		RemoveHead();
}

template<class T>
inline void Chain<T>::AddHead(T& component)
{
	Node<T>* node = new Node<T>();
	node->component = component;
	node->next = NULL;

	if (m_head == NULL)
		m_head = node;
	else
	{
		node->next = m_head;
		m_head = node;
	}
	m_size++;
}

template<class T>
inline void Chain<T>::AddTail(T& component)
{
	Node<T>* node = new Node<T>();
	node->component = component;
	node->next = NULL;

	if (m_head == NULL)
		m_head = node;
	else
	{
		Node<T>* last = m_head;
		while (last->next != NULL)
			last = GetNext(*last);
		last->next = node;
	}
	m_size++;
}

template<class T>
inline void Chain<T>::InsertBefore(T& component, Node<T>* node)
{
	if (node == m_head || m_head == NULL)
	{
		AddHead(component);
		return;
	}

	Node<T>* previous = m_head;
	while (previous->next->next != NULL)
	{
		if (previous->next->next == node)
		{
			Node<T>* temp = new Node<T>();
			temp->component = component;
			temp->next = previous->next->next;
			previous->next = temp;
			return;
		}
		previous = GetNext(*previous);
	}
}

template<class T>
inline void Chain<T>::InsertAt(T& component, int pos)
{
	if (pos == 0)
	{
		AddHead(component);
		return;
	}
	if (pos > m_size)
		return;

	Node<T>* node = m_head;
	while (pos--)
	{
		if (pos == 0)
		{
			Node<T>* temp = new Node<T>();
			temp->component = component;
			temp->next = node->next;
			node->next = temp;
		}
		else node = GetNext(*node);
	}
	m_size++;
}

template<class T>
inline void Chain<T>::RemoveHead()
{
	if (m_head == NULL)
		return;

	Node<T>* temp = m_head;
	m_head = m_head->next;
	delete temp;
	m_size--;
}

template<class T>
inline void Chain<T>::RemoveTail()
{
	if (m_head == NULL)
		return;
	if (m_head->next == NULL)
	{
		delete m_head;
		m_head = NULL;
		m_size--;
		return;
	}

	Node<T>* previous = m_head;
	while (previous->next->next != NULL)
		previous = GetNext(*previous);
	delete (previous->next);
	previous->next = NULL;
	m_size--;
}

template<class T>
inline void Chain<T>::Remove(Node<T>* node)
{
	if (m_head == node)
	{
		if (m_head->next == NULL)
		{
			delete m_head;
			m_head = NULL;
		}
		else
		{
			m_head->component = m_head->next->component;
			Node<T>* temp = m_head->next;
			m_head->next = m_head->next->next;
			delete temp;
		}
		m_size--;
		return;
	}
	Node<T>* previous = m_head;
	while (previous->next != NULL && previous->next != node)
		previous = previous->next;
	if (previous->next == NULL)
		return;

	previous->next = previous->next->next;
	delete(node);
	m_size--;
}

template<class T>
inline Node<T>* Chain<T>::GetHead() const
{
	return m_head;
}

template<class T>
inline Node<T>* Chain<T>::GetTail() const
{
	Node<T>* last = m_head;
	while (last->next != NULL)
		last = GetNext(*last);
	return last;
}

template<class T>
inline Node<T>* Chain<T>::GetNext(Node<T> current) const
{
	return current.next;
}

template<class T>
inline int Chain<T>::GetSize() const
{
	return m_size;
}

#endif