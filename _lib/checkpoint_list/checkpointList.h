#ifndef CHECK_POINT_LIST_H__
#define CHECK_POINT_LIST_H__

template<class T>
class CheckpointList
{
public:
	template<class U>
	struct Node
	{
		Node(const U& data, Node<U>* next) : data(data), next(next) { }
		U data;
		Node<U>* next;
	};
	template<class U>
	struct CheckpointNode
	{
		CheckpointNode(Node<U>* node, CheckpointNode<U>* next) : node(node), next(next) { }
		Node<U>* node;
		CheckpointNode<U>* next;
	};
	template<class U>
	struct TabNode
	{
		TabNode(TabNode<U>* next) : next(next) { }
		CheckpointNode<U>* checkpointNode = 0;
		TabNode<U>* next;
	};
	CheckpointList();
	CheckpointList(CheckpointList<T>& source);
	~CheckpointList();
	int GetSize() const;
	Node<T>* GetNodeHead() const;
	TabNode<T>* GetTabHead() const;
	Node<T>* GetNextNode(const Node<T>* node) const;
	TabNode<T>* GetNextTab(const TabNode<T>* node) const;
	CheckpointNode<T>* GetNextCheckpoint(const CheckpointNode<T>* node) const;
	Node<T>* InsertAfterNode(const T& data, Node<T>* node);
	TabNode<T>* InsertAfterTab(TabNode<T>* tabNode);
	CheckpointNode<T>* InsertAfterCheckpoint(Node<T>* node, CheckpointNode<T>* checkpointNode, TabNode<T>* tabNode);
	void Clear();
private:
	int m_size;
	Node<T>* m_head;
	TabNode<T>* m_tabHead;
};

template<class T>
inline CheckpointList<T>::CheckpointList()
{
	m_size = 0;
	m_head = 0;
	m_tabHead = 0;
}

template<class T>
inline CheckpointList<T>::CheckpointList(CheckpointList<T>& source)
{
	m_head = 0;
	m_tabHead = 0;
	m_size = source.m_size;
	
	Node<T>* nodeIt = source.m_head;
	while(nodeIt != 0)
	{
		//Duplicate nodes
		Node<T>* temp = new Node<T>(nodeIt->data, nodeIt->next);
		nodeIt->next = temp;
		nodeIt = temp->next;
	}
	TabNode<T>* tabIt = source.m_tabHead;
	while(tabIt != 0)
	{
		CheckpointNode<T>* checkpointIt = tabIt->checkpointNode;
		while(checkpointIt != 0)
		{
			//Duplicate checkpointNodes and mimic their node pointers
			CheckpointNode<T>* temp = new CheckpointNode<T>(checkpointIt->node->next, checkpointIt->next);
			checkpointIt->next = temp;
			checkpointIt = temp->next;
		}
		//Duplicate tabNodes and mimic their checkPointNode pointers
		TabNode<T>* temp = new TabNode<T>(tabIt->next);
		temp->checkpointNode = tabIt->checkpointNode->next;
		tabIt->next = temp;
		tabIt = temp->next;
	}
	//Separate lists
	nodeIt = source.m_head;
	m_head = nodeIt->next;
	while(nodeIt != 0)
	{
		Node<T>* temp = nodeIt->next;
		nodeIt->next = temp->next;
		temp->next = temp->next == 0 ? 0 :temp->next->next;
		nodeIt = nodeIt->next;
	}
	tabIt = source.m_tabHead;
	m_tabHead = tabIt->next;
	while(tabIt != 0)
	{
		CheckpointNode<T>* checkpointIt = tabIt->checkpointNode;
		while(checkpointIt != 0)
		{
			CheckpointNode<T>* temp = checkpointIt->next;
			checkpointIt->next = temp->next;
			temp->next = temp->next == 0 ? 0 : temp->next->next;
			checkpointIt = checkpointIt->next;
		}
		TabNode<T>* temp = tabIt->next;
		tabIt->next = temp->next;
		temp->next = temp->next == 0 ? 0 :temp->next->next;
		tabIt = tabIt->next;
	}
}

template<class T>
inline CheckpointList<T>::~CheckpointList()
{
	Clear();
	delete m_head;
	delete m_tabHead;
}

template<class T>
inline int CheckpointList<T>::GetSize() const
{
	return m_size;
}

template<class T>
inline CheckpointList<T>::Node<T>* CheckpointList<T>::GetNodeHead() const
{
	return m_head;
}

template<class T> 
inline CheckpointList<T>::TabNode<T>* CheckpointList<T>::GetTabHead() const
{
	return m_tabHead;
}

template<class T>
inline CheckpointList<T>::Node<T>* CheckpointList<T>::GetNextNode(const Node<T>* node) const
{
	return node == 0 ? 0 : node->next;
}

template<class T>
inline CheckpointList<T>::TabNode<T>* CheckpointList<T>::GetNextTab(const TabNode<T>* node) const
{
	return node == 0 ? 0 : node->next;
}

template<class T>
inline CheckpointList<T>::CheckpointNode<T>* CheckpointList<T>::GetNextCheckpoint(const CheckpointNode<T>* node) const
{
	return node == 0 ? 0 : node->next;
}

template<class T>
inline CheckpointList<T>::Node<T>* CheckpointList<T>::InsertAfterNode(const T& data, Node<T>* node)
{
	Node<T>* temp;
	if(node == m_head)
	{
		if (m_head == 0)
		{
			temp = new Node<T>(data, 0);
			m_head = temp;
		}
		else
		{
			temp = new Node<T>(data, m_head->next);
			m_head->next = temp;
		}
		m_size++;
		return temp;
	}

	temp = new Node<T>(data, node->next);
	node->next = temp;
	m_size++;
	return temp;
}

template<class T>
inline CheckpointList<T>::TabNode<T>* CheckpointList<T>::InsertAfterTab(TabNode<T>* tabNode)
{
	TabNode<T>* temp;
	if(tabNode == m_tabHead)
	{
		if(m_tabHead == 0)
		{
			temp = new TabNode<T>(0);
			m_tabHead = temp;
		}	
		else
		{
			temp = new TabNode<T>(m_tabHead->next);
			m_tabHead->next = temp;
		}
		return temp;
	}

	temp = new TabNode<T>(tabNode->next);
	tabNode->next = temp;
	return temp;
}

template<class T> 
inline CheckpointList<T>::CheckpointNode<T>* CheckpointList<T>::InsertAfterCheckpoint(Node<T>* node, CheckpointNode<T>* checkpointNode, TabNode<T>* tabNode)
{
	CheckpointNode<T>* temp;
	if(checkpointNode == tabNode->checkpointNode)
	{
		if(tabNode->checkpointNode == 0)
		{
			temp = new CheckpointNode<T>(node, 0);
			tabNode->checkpointNode = temp;
		}
		else
		{
			temp = new CheckpointNode<T>(node, tabNode->checkpointNode->next);
			tabNode->checkpointNode->next = temp;
		}
		return temp;
	}

	temp = new CheckpointNode<T>(node, checkpointNode->next);
	checkpointNode->next = temp;
	return temp;
}

template<class T>
inline void CheckpointList<T>::Clear()
{
	while(m_head != 0)
	{
		Node<T>* temp = m_head;
		m_head = m_head->next;
		delete temp;
		m_size--;
	}
	while(m_tabHead != 0)
	{
		CheckpointNode<T>* checkpointIt = m_tabHead->checkpointNode;
		while(checkpointIt != 0)
		{
			CheckpointNode<T>* temp = checkpointIt;
			checkpointIt = checkpointIt->next;
			delete temp;
		}
		TabNode<T>* temp = m_tabHead;
		m_tabHead = m_tabHead->next;
		delete temp;
	}
	m_head = 0;
	m_tabHead = 0;
}

#endif