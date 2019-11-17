#ifndef QUEUE_H__
#define QUEUE_H__

template <typename T>
class Queue
{
public:
    template <typename U>
    struct Iterator
    {
    public:
        Iterator(const Queue<T> &source) : capacity(source.m_capacity), front(source.m_front) { pos = front + 1; }
        int GetPos() { return pos; }
        bool End() { return pos == capacity; }
        int Next()
        {
            if (pos < capacity)
                pos++;
            return pos;
        }

    public:
        const int capacity, front;

    private:
        int pos;
    };
    Queue();
    Queue(const int &);
    ~Queue();
    void operator=(const Queue &source);
    void Push(const T &element);
    T Pop();
    T Front() const;
    T Get(Iterator<T> &iterator) const;
    int GetFrontPosition() const;
    int GetSize() const;
    int GetCapacity() const;

private:
    int m_front, m_capacity, m_size;
    T *m_data;
};

template <typename T>
inline Queue<T>::Queue()
{
    m_capacity = 4;
    m_front = m_capacity - 1;
    m_size = 0;
    m_data = new T[m_capacity];
}

template <typename T>
inline Queue<T>::Queue(const int &capacity)
{
    m_capacity = capacity;
    m_front = m_capacity - 1;
    m_size = 0;
    m_data = new T[m_capacity];
}

template <typename T>
inline Queue<T>::~Queue()
{
    delete[] m_data;
}

template <typename T>
inline void Queue<T>::operator=(const Queue<T> &source)
{
    this->m_data = source.m_data;
    this->m_capacity = source.m_capacity;
    this->m_size = source.m_size;
    this->m_front = source.m_front;
}

template <typename T>
inline void Queue<T>::Push(const T &element)
{
    if (m_size == m_capacity)
    {
        m_front += m_size;
        m_capacity = m_capacity * 2;
        T *data = new T[m_capacity];
        for (int i = 0; i < m_size; i++)
            data[m_capacity - 1 - i] = m_data[m_size - 1 - i];
        m_data = data;
    }
    m_data[m_front] = element;
    m_size++;
    m_front--;
}

template <typename T>
inline T Queue<T>::Pop()
{
    m_size--;
    m_front++;
    return m_data[m_front];
}

template <typename T>
inline T Queue<T>::Front() const
{
    return m_data[m_front + 1];
}

template <typename T>
inline T Queue<T>::Get(Iterator<T> &iterator) const
{
    return m_data[iterator.GetPos()];
}

template <typename T>
inline int Queue<T>::GetFrontPosition() const
{
    return m_front;
}

template <typename T>
inline int Queue<T>::GetSize() const
{
    return m_size;
}

template <typename T>
inline int Queue<T>::GetCapacity() const
{
    return m_capacity;
}

#endif