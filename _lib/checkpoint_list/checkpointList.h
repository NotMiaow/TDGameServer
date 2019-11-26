#ifndef CHECK_POINT_LIST_H__
#define CHECK_POINT_LIST_H__

#include "dataNode.h"
#include "checkpointNode.h"
#include "tabNode.h"

template<class T>
class CheckpointList
{
public:
	class Iterator
	{
	public:
		Iterator(DataNode<T>* start, DataNode<T>* end) : m_curNode(start), m_endNode(end) { }
		void operator++(int) { m_curNode = m_curNode->next; }
		T& Get() { return m_curNode->data; } 
		bool End() { return m_curNode == m_endNode; }
	private:
		DataNode<T>* m_curNode;
		DataNode<T>* m_endNode;
	};
	CheckpointList();
	~CheckpointList();
	DataNode<T>* GetNodeHead() const;
	TabNode<T>* GetTabHead() const;
	DataNode<T>* GetNextNode(const DataNode<T>* node) const;
	CheckpointNode<T>* GetNextCheckpoint(const CheckpointNode<T>* node) const;
	TabNode<T>* GetNextTab(const TabNode<T>* node) const;
	void InsertNode(const T& data, CheckpointNode<T>* checkpointNode);
	CheckpointNode<T>* InsertCheckpoint(CheckpointNode<T>* checkpointNode);
	TabNode<T>* InsertTab(TabNode<T>* tabNode);
	void Clear();
private:
	DataNode<T>* InsertNodeAfter(const T& data, DataNode<T>* node);
private:
	DataNode<T>* m_head;
	CheckpointNode<T>* m_checkpointHead;
	TabNode<T>* m_tabHead;
};

template<class T>
inline CheckpointList<T>::CheckpointList()
{
	m_head = 0;
	m_checkpointHead = 0;
	m_tabHead = 0;
}

template<class T>
inline CheckpointList<T>::~CheckpointList()
{
	Clear();
}

template<class T>
inline DataNode<T>* CheckpointList<T>::GetNodeHead() const
{
	return m_head;
}

template<class T> 
inline TabNode<T>* CheckpointList<T>::GetTabHead() const
{
	return m_tabHead;
}

template<class T>
inline DataNode<T>* CheckpointList<T>::GetNextNode(const DataNode<T>* node) const
{
	return node == 0 ? 0 : node->next;
}

template<class T>
inline TabNode<T>* CheckpointList<T>::GetNextTab(const TabNode<T>* node) const
{
	return node == 0 ? 0 : node->next;
}

template<class T>
inline CheckpointNode<T>* CheckpointList<T>::GetNextCheckpoint(const CheckpointNode<T>* node) const
{
	return node == 0 ? 0 : node->next;
}

template<class T>
inline DataNode<T>* CheckpointList<T>::InsertNodeAfter(const T& data, DataNode<T>* node)
{
	DataNode<T>* temp;
	if (m_head == 0)
	{
		m_head = new DataNode<T>(data, 0);
		return m_head;
	}

	temp = new DataNode<T>(data, node->next);
	node->next = temp;
	return temp;
}

template<class T>
inline void CheckpointList<T>::InsertNode(const T& data, CheckpointNode<T>* checkpointNode)
{
	if(checkpointNode->node == 0)
	{
		if(checkpointNode == m_checkpointHead)
			m_checkpointHead->node = InsertNodeAfter(data, m_checkpointHead->node);
		else
		{
			CheckpointNode<T>* prev = m_checkpointHead;
			DataNode<T>* lastNode = m_head;
			//Until given checkpoint is reached
			while(prev->next && prev != checkpointNode)
			{
				//If a checkpoint with a node is found
				if(prev->node)
					lastNode = prev->node;
				prev = prev->next;
			}
			//Get the last node of this checkpoint
			while(lastNode && lastNode->next)
				lastNode = lastNode->next;
			checkpointNode->node = InsertNodeAfter(data, lastNode);
		}
	}
	else InsertNodeAfter(data, checkpointNode->node);
}

template<class T> 
inline CheckpointNode<T>* CheckpointList<T>::InsertCheckpoint(CheckpointNode<T>* checkpointNode)
{
	CheckpointNode<T>* temp = new CheckpointNode<T>(checkpointNode->next);
	checkpointNode->next = temp;
	return temp;
}

template<class T>
inline TabNode<T>* CheckpointList<T>::InsertTab(TabNode<T>* tabNode)
{
	if(m_tabHead == 0)
	{
		m_tabHead = new TabNode<T>();
		m_checkpointHead = m_tabHead->checkpointNode;
		return m_tabHead;
	}

	TabNode<T>* temp = new TabNode<T>();
	temp->next = tabNode->next;
	temp->checkpointNode->next = tabNode->checkpointNode->next;
	tabNode->next = temp;
	tabNode->checkpointNode->next = temp->checkpointNode;
	return temp;
}

template<class T>
inline void CheckpointList<T>::Clear()
{
	while(m_tabHead != 0)
	{
		TabNode<T>* temp = m_tabHead;
		m_tabHead = m_tabHead->next;
		delete temp;
	}
	while(m_checkpointHead != 0)
	{
		CheckpointNode<T>* temp = m_checkpointHead;
		m_checkpointHead = m_checkpointHead->next;
		delete temp;
	}
	while(m_head != 0)
	{
		DataNode<T>* temp = m_head;
		m_head = m_head->next;
		delete temp;
	}
	m_head = 0;
	m_checkpointHead = 0;
	m_tabHead = 0;
}

#endif