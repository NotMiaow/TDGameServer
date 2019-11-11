#ifndef CHAIN_H__
#define CHAIN_H__

#include <iostream>

template <class T>
class Chain
{
public:
	template <class U>
	struct Node
	{
		U data;
		Node<U>* next;
	};
	Chain();
	Chain(const Chain& source);
	~Chain();
	void AddHead(T& component);
	void AddTail(T& component);
	void RemoveHead();
	void RemoveTail();
	Node<T>* GetHead() const;
	Node<T>* GetTail() const;
	Node<T>* GetNext(Node<T> current) const;
	int GetSize() const;
private:
	int m_size;
	Node<T>* m_head;
	Node<T>* m_tail;
};

template<class T>
inline Chain<T>::Chain()
{
	m_head = 0;
	m_tail = 0;
	m_size = 0;
}

template<class T>
inline Chain<T>::Chain(const Chain& source) : m_head(NULL)
{
	Node<T>* node = source.GetHead();
	while (node != NULL)
	{
		AddTail(node->data);
		node = source.GetNext(*node);
	}
	m_size = source.m_size;
}

template<class T>
inline Chain<T>::~Chain()
{
	while (m_head != 0)
		RemoveHead();
}

template<class T>
inline void Chain<T>::AddHead(T& component)
{
	Node<T>* node = new Node<T>();
	node->data = component;
	node->next = 0;

	if (m_head == 0)
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
	node->data = component;
	node->next = 0;

	if (m_head == 0)
		m_head = node;
	else
	{
		Node<T>* last = m_head;
		while (last->next != 0)
			last = GetNext(*last);
		last->next = node;
	}
	m_size++;
}

template<class T>
inline void Chain<T>::RemoveHead()
{
	if (m_head == 0)
		return;

	Node<T>* temp = m_head;
	m_head = m_head->next;
	delete temp;
	m_size--;
}

template<class T>
inline void Chain<T>::RemoveTail()
{
	if (m_head == 0)
		return;
	if (m_head->next == 0)
	{
		delete m_head;
		m_head = 0;
		m_size--;
		return;
	}

	Node<T>* previous = m_head;
	while (previous->next->next != 0)
		previous = GetNext(*previous);
	delete (previous->next);
	previous->next = 0;
	m_size--;
}

template<class T>
inline Chain<T>::Node<T>* Chain<T>::GetHead() const
{
	return m_head;
}

template<class T>
inline Chain<T>::Node<T>* Chain<T>::GetTail() const
{
	Node<T>* last = m_head;
	while (last->next != 0)
		last = GetNext(*last);
	return last;
}

template<class T>
inline Chain<T>::Node<T>* Chain<T>::GetNext(Node<T> current) const
{
	return current.next;
}

template<class T>
inline int Chain<T>::GetSize() const
{
	return m_size;
}

#endif