#ifndef QUEUE_H__
#define QUEUE_H__

#include <iostream>
#include <mutex>

template<class T>
class Queue
{
public:
	template <class U>
	struct Node
	{
		U component;
		Node<U>* next;
	};
	Queue();
	void QueueUp(T& component);
	T GetComponent();
	bool IsNotEmpty();
private:
	Node<T>* m_head = NULL;
	Node<T>* m_tail = NULL;
	Node<T>* m_temp;
	std::mutex m_mutex;
};

template<class T>
inline Queue<T>::Queue()
{
	m_head = NULL;
	m_tail = NULL;
}

template<class T>
void Queue<T>::QueueUp(T& component)
{
	m_mutex.lock();
	if (m_head == NULL) {
		m_tail = new Node<T>();
		m_tail->next = NULL;
		m_tail->component = component;
		m_head = m_tail;
	}
	else {
		m_temp = new Node<T>();
		m_tail->next = m_temp;
		m_temp->component = component;
		m_temp->next = NULL;
		m_tail = m_temp;
	}
	m_mutex.unlock();
}

template<class T>
T Queue<T>::GetComponent()
{
	m_mutex.lock();
	m_temp = m_head;
	if (m_head == NULL) {
		m_mutex.unlock();
		return nullptr;
	}
	else
	{
		T a;
		if (m_temp->next != NULL) {
			m_temp = m_temp->next;
			a = m_head->component;
			m_head = m_temp;
		}
		else {
			a = m_head->component;
			m_head = NULL;
			m_tail = NULL;
		}
		m_mutex.unlock();
		return a;
	}
}

template<class T>
inline bool Queue<T>::IsNotEmpty()
{
	if (m_head == NULL)
		return false;
	return true;
}

#endif