#ifndef SHARED_QUEUE_H__
#define SHARED_QUEUE_H__

#include <mutex>

template <typename T>
class SharedQueue
{
public:
    template<typename U>
    struct Iterator
    {
    public:
        Iterator(const SharedQueue<T>& source) : capacity(source.m_capacity), front(source.m_front) { pos = front + 1; }
        int GetPos() { return pos; }
        bool End() { return pos == capacity; }
        int Next() { if(pos < capacity) pos++; return pos; }
    public:
        const int capacity, front;
    private:
        int pos;
    };
    SharedQueue();
    SharedQueue(const int&);
    ~SharedQueue();
    void operator=(const SharedQueue& source);
    void Push(const T& element);
    T Pop();
    T Front() const;
    T Get(Iterator<T>& iterator) const;
    int GetFrontPosition() const;
    int GetSize() const;
    int GetCapacity() const;
private:
    int m_front, m_capacity, m_size;
    T* m_data;
    std::mutex m_mutex;
};

template<typename T>
inline SharedQueue<T>::SharedQueue()
{
    m_capacity = 4;
    m_front = m_capacity - 1;
    m_size = 0;
    m_data = new T[m_capacity];
}

template<typename T>
inline SharedQueue<T>::SharedQueue(const int& capacity)
{
    m_capacity = capacity;
    m_front = m_capacity - 1;
    m_size = 0;
    m_data = new T[m_capacity];
}

template<typename T>
inline SharedQueue<T>::~SharedQueue()
{
    delete[] m_data;
}

template<typename T>
inline void SharedQueue<T>::operator=(const SharedQueue<T>& source)
{
    this->m_data = source.m_data;
    this->m_capacity = source.m_capacity;
    this->m_size = source.m_size;
    this->m_front = source.m_front;
}

template<typename T>
inline void SharedQueue<T>::Push(const T& element)
{
    m_mutex.lock();
    if(m_size == m_capacity)
    {
        m_front += m_size;
        m_capacity = m_capacity * 2;
        T* data = new T[m_capacity];
        for(int i = 0; i < m_size; i++)
            data[m_capacity - 1 - i] = m_data[m_size - 1 - i];
        delete[] m_data;
        m_data = data;
    }
    m_data[m_front] = element;
    m_size++;
    m_front--;
    m_mutex.unlock();
}

template<typename T>
inline T SharedQueue<T>::Pop()
{
    m_mutex.lock();
    if(m_size == 0)
    {
        m_mutex.unlock();
        return nullptr;
    }
    m_size--;
    m_front++;
    m_mutex.unlock();
    return m_data[m_front];
}

template<typename T>
inline T SharedQueue<T>::Front() const
{
    return m_data[m_front + 1];
}

template<typename T>
inline T SharedQueue<T>::Get(Iterator<T>& iterator) const
{
    return m_data[iterator.GetPos()];
}

template<typename T>
inline int SharedQueue<T>::GetFrontPosition() const
{
    return m_front;
}

template<typename T>
inline int SharedQueue<T>::GetSize() const
{
    return m_size;
}

template<typename T>
inline int SharedQueue<T>::GetCapacity() const
{
    return m_capacity;
}

#endif