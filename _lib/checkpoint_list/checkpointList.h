#ifndef CHECK_POINT_LIST_H__
#define CHECK_POINT_LIST_H__

#include <iostream>

#include "dataNode.h"
#include "checkpointNode.h"
#include "tabNode.h"

template <class T>
class CheckpointList
{
public:
	class Iterator
	{
	public:
		Iterator(DataNode<T> *start, DataNode<T> *end) : m_index(0), m_curNode(start), m_endNode(end) {}
		void operator++(int) { m_index++; m_curNode = m_curNode->next; }
		T* Get() const { return &m_curNode->data; }
		bool End() const { return m_curNode == m_endNode;	}
		const int Index() const { return m_index; }
	private:
		int m_index;
		DataNode<T> *m_curNode;
		DataNode<T> *m_endNode;
	};
	CheckpointList();
	~CheckpointList();
	void Init(const int &tabCount, const int &checkpointCount);
	DataNode<T> *GetNodeHead() const;
	auto GetIterator(const int &tabIndex, const int &checkpointIndex);
	T* GetData(const int& index, const int& tabIndex, const int& checkpointIndex) const;
	void InsertNode(const T &data, const int &tabIndex, const int &checkpointIndex);
	void RemoveNode(const int& index, const int& tabIndex, const int& checkpointIndex);
private:
	void RemoveAtCheckpointHead(CheckpointNode<T>* checkpointNode);
	CheckpointNode<T>* GetCheckpoint(TabNode<T>* tabIt, const int& checkpointIndex);
	CheckpointNode<T>* GetPreviousNotEmptyCheckpoint(CheckpointNode<T>* currentCheckpoint);
	CheckpointNode<T>* GetNextNotEmptyCheckpoint(CheckpointNode<T>* currentCheckpoint);
	DataNode<T>* GetLastNodeOfCheckpoint(CheckpointNode<T>* checkpointNode);
	CheckpointNode<T>* InsertCheckpoint(CheckpointNode<T> *checkpointNode);
	TabNode<T> *InsertTab(TabNode<T> *tabNode);
	void Clear();

private:
	DataNode<T> *InsertNodeAfter(const T &data, DataNode<T> *node);

private:
	DataNode<T> *m_head;
	CheckpointNode<T> *m_checkpointHead;
	TabNode<T> *m_tabHead;
	int m_size;
};

template <class T>
inline CheckpointList<T>::CheckpointList()
{
	m_head = 0;
	m_checkpointHead = 0;
	m_tabHead = 0;
	m_size = 0;
}

template <class T>
inline CheckpointList<T>::~CheckpointList()
{
	Clear();
}

template <class T>
inline void CheckpointList<T>::Init(const int &tabCount, const int &checkpointCount)
{
	TabNode<T> *tabIt = m_tabHead;
	CheckpointNode<T> *checkpointIt;
	for (int i = 0; i < tabCount; i++)
	{
		tabIt = InsertTab(tabIt);
		checkpointIt = tabIt->checkpointNode;
		for (int j = 1; j < checkpointCount; j++)
			checkpointIt = InsertCheckpoint(checkpointIt);
	}
}

template <class T>
inline DataNode<T> *CheckpointList<T>::GetNodeHead() const
{
	return m_head;
}

template <class T>
inline auto CheckpointList<T>::GetIterator(const int &tabIndex, const int &checkpointIndex)
{
	//Get tabNode
	TabNode<T> *tabNode = m_tabHead;
	for (int i = 0; i < tabIndex; i++, tabNode = tabNode->next);
	
	//Get checkpointNode
	CheckpointNode<T> *checkpointIt = tabNode->checkpointNode;
	for (int i = 0; i < checkpointIndex; i++, checkpointIt = checkpointIt->next);
	
	//Find end of block
	CheckpointNode<T> *temp = checkpointIt->next;
	while (temp->next && temp->node == 0)
		temp = temp->next;

	//Create and return iterator
	Iterator iterator(checkpointIt->node, temp->node);
	return iterator;
}

template<class T>
inline T* CheckpointList<T>::GetData(const int& index, const int& tabIndex, const int& checkpointIndex) const
{
	//Get the tab at requested index
	TabNode<T>* tabIt = m_tabHead;
	for(int i = 0; i < tabIndex; i++, tabIt = tabIt->next);
	//Get the requested checkpoint within the requested tab
	CheckpointNode<T>* checkpointIt = tabIt->checkpointNode;
	for(int i = 0; i < checkpointIndex; i++, checkpointIt = checkpointIt->next);
	//Get the requested node within requested checkpoint
	DataNode<T>* nodeIt = checkpointIt->node;
	for(int i = 0; nodeIt && i < index; i++, nodeIt = nodeIt->next);
	return nodeIt ? &nodeIt->data : 0;
}

template <class T>
inline DataNode<T> *CheckpointList<T>::InsertNodeAfter(const T &data, DataNode<T> *node)
{
	DataNode<T> *temp;
	if (m_head == 0)
	{
		m_head = new DataNode<T>(data, 0);
		m_size++;
		return m_head;
	}

	temp = new DataNode<T>(data, node->next);
	node->next = temp;
	m_size++;
	return temp;
}

template <class T>
inline void CheckpointList<T>::RemoveNode(const int& index, const int& tabIndex, const int& checkpointIndex)
{
	//Get the tab at requested index
	TabNode<T>* tabIt = m_tabHead;
	for(int i = 0; i < tabIndex; i++, tabIt = tabIt->next);
	//Get the requested checkpoint within the requested tab
	CheckpointNode<T>* checkpointIt = tabIt->checkpointNode;
	for(int i = 0; i < checkpointIndex; i++, checkpointIt = checkpointIt->next);
	if(checkpointIt->node == 0)
	{
		std::cout << "Attempted removing on an empty checkpoint (0," << tabIndex << ","<< checkpointIndex << ")" << std::endl;
		return;
	}

	if(index == 0)
	{
		if(tabIndex == 0)
		{
			if(checkpointIndex == 0 && m_head)
			{
				CheckpointNode<T>* nextNotEmpty = GetNextNotEmptyCheckpoint(checkpointIt);
				DataNode<T>* temp = m_head;
				m_checkpointHead->node = (nextNotEmpty && m_head->next == nextNotEmpty->node ? 0 : m_head->next);
				m_head = m_head->next;
				delete temp;
				m_size--;
				std::cout << "Removed (0,0,0)" << std::endl;
			}
			else RemoveAtCheckpointHead(checkpointIt);
		}
		else RemoveAtCheckpointHead(checkpointIt);
	}
	else
	{
		//Get the requested node within requested checkpoint
		DataNode<T>* nodeIt = checkpointIt->node;
		for(int i = 0; i < index - 1; i++, nodeIt = nodeIt->next);
		DataNode<T>* temp = nodeIt->next;
		nodeIt->next = nodeIt->next->next;
		delete temp;
		m_size--;
		std::cout << "Removed (x,x,x)" << std::endl;
	}
}

template<class T>
inline void CheckpointList<T>::RemoveAtCheckpointHead(CheckpointNode<T>* checkpointNode)
{
	CheckpointNode<T>* previousNotEmpty = GetPreviousNotEmptyCheckpoint(checkpointNode);
	if(previousNotEmpty)
	{
		DataNode<T>* previous;
		if(previousNotEmpty->node->next == checkpointNode->node)
			previous = previousNotEmpty->node;
		else previous = GetLastNodeOfCheckpoint(previousNotEmpty);
		DataNode<T>* temp = checkpointNode->node;
		previous->next = checkpointNode->node->next;
		checkpointNode->node = checkpointNode->node->next;
		delete temp;
		m_size--;
		std::cout << "Removed (0,x,x)" << std::endl;
	}
	else if(m_head)
	{
		CheckpointNode<T>* nextNotEmpty = GetNextNotEmptyCheckpoint(checkpointNode);
		DataNode<T>* temp = m_head;
		checkpointNode->node = (nextNotEmpty && m_head->next == nextNotEmpty->node ? 0 : m_head->next);
		m_head = m_head->next;
		delete temp;
		m_size--;
		std::cout << "Removed (0,x,x)***which happened to be m_head****" << std::endl;
	}
	else std::cout << "Attempted removing on an empty checkpoint (0,x,x)" << std::endl;
}

template<class T>
inline void CheckpointList<T>::InsertNode(const T &data, const int &tabIndex, const int &checkpointIndex)
{
	//Get the tab at requested index
	TabNode<T>* tabIt = m_tabHead;
	for(int i = 0; i < tabIndex && tabIt; i++, tabIt = tabIt->next);
	//Get the requested checkpoint within the requested tab
	CheckpointNode<T>* checkpointIt = tabIt->checkpointNode;
	for(int i = 0; i < checkpointIndex && checkpointIt; i++, checkpointIt = checkpointIt->next);

	//Insert node and update links
	if (checkpointIt->node == 0)
	{
		if (checkpointIt == m_checkpointHead)
			m_checkpointHead->node = InsertNodeAfter(data, m_checkpointHead->node);
		else
		{
			CheckpointNode<T> *prev = m_checkpointHead;
			DataNode<T> *lastNode = m_head;
			//Until given checkpoint is reached
			while (prev->next && prev != checkpointIt)
			{
				//If a checkpoint with a node is found
				if (prev->node)
					lastNode = prev->node;
				prev = prev->next;
			}
			//Get the last node of this checkpoint
			while (lastNode && lastNode->next)
				lastNode = lastNode->next;
			checkpointIt->node = InsertNodeAfter(data, lastNode);
		}
	}
	else
		InsertNodeAfter(data, checkpointIt->node);
}

template<class T>
inline CheckpointNode<T>* CheckpointList<T>::GetCheckpoint(TabNode<T>* tabIt, const int& checkpointIndex)
{
	CheckpointNode<T>* checkpointIt = tabIt->checkpointNode;
	for(int i = 0; i < checkpointIndex; i++, checkpointIt = checkpointIt->next);
	return checkpointIt;
}

template<class T>
inline CheckpointNode<T>* CheckpointList<T>::GetPreviousNotEmptyCheckpoint(CheckpointNode<T>* currentCheckpoint)
{
	CheckpointNode<T>* prev = m_checkpointHead;
	CheckpointNode<T>* previousNotEmpty = 0;
	while(prev->next != currentCheckpoint)
	{
		if(prev->node)
			previousNotEmpty = prev;
		prev = prev->next;
	}
	return prev->node ? prev : 0;
}

template<class T>
inline CheckpointNode<T>* CheckpointList<T>::GetNextNotEmptyCheckpoint(CheckpointNode<T>* currentCheckpoint)
{
	CheckpointNode<T>* cur = currentCheckpoint;
	while(cur->next && currentCheckpoint->node->next != cur->next->node)
		cur = cur->next;
	return cur->next;
}

template<class T>
inline DataNode<T>* CheckpointList<T>::GetLastNodeOfCheckpoint(CheckpointNode<T>* checkpointNode)
{
	DataNode<T>* lastNode = checkpointNode->node;
	while(lastNode->next != checkpointNode->next->node)
		lastNode = lastNode->next;
	return lastNode;
}

template <class T>
inline CheckpointNode<T> *CheckpointList<T>::InsertCheckpoint(CheckpointNode<T> *checkpointNode)
{
	CheckpointNode<T> *temp = new CheckpointNode<T>(checkpointNode->next);
	checkpointNode->next = temp;
	return temp;
}

template <class T>
inline TabNode<T> *CheckpointList<T>::InsertTab(TabNode<T> *tabNode)
{
	if (m_tabHead == 0)
	{
		m_tabHead = new TabNode<T>();
		m_checkpointHead = m_tabHead->checkpointNode;
		return m_tabHead;
	}

	TabNode<T> *temp = new TabNode<T>();
	temp->next = tabNode->next;
	temp->checkpointNode->next = tabNode->checkpointNode->next;
	tabNode->next = temp;
	tabNode->checkpointNode->next = temp->checkpointNode;
	return temp;
}

template <class T>
inline void CheckpointList<T>::Clear()
{
	while (m_tabHead)
	{
		TabNode<T> *temp = m_tabHead;
		m_tabHead = m_tabHead->next;
		delete temp;
	}
	while (m_checkpointHead)
	{
		CheckpointNode<T> *temp = m_checkpointHead;
		m_checkpointHead = m_checkpointHead->next;
		delete temp;
	}
	while (m_head)
	{
		DataNode<T> *temp = m_head;
		m_head = m_head->next;
		delete temp;
	}
	m_head = 0;
	m_checkpointHead = 0;
	m_tabHead = 0;
}

#endif