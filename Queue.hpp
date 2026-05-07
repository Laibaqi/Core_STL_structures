#ifndef QUEUE_HPP
#define QUEUE_HPP

#include<stdexcept>
#include<cstddef>

template <typename T>
class Queue {
private:
    T* data;
    size_t _front;  
    size_t _rear;     
    size_t _count;    
    size_t _capacity; 

    void resize() {
        size_t newCapacity = (_capacity == 0) ? 1 : _capacity * 2;
        T* newData = new T[newCapacity];

        for (size_t i = 0; i < _count; i++) {
            newData[i] = data[(_front + i) % _capacity];
        }

        delete[] data;
        data = newData;
        _front = 0;
        _rear = _count;
        _capacity = newCapacity;
    }

public:
    Queue() : data(nullptr), _front(0), _rear(0), _count(0), _capacity(0) {}

    ~Queue() {
        delete[] data;
    }

    void enqueue(const T& value) {
        if (_count >= _capacity) {
            resize();
        }
        data[_rear] = value;
        _rear = (_rear + 1) % _capacity; 
        _count++;
    }

    void dequeue() {
        if (isEmpty()) {
            throw std::out_of_range("Queue Underflow: Cannot dequeue from empty queue");
        }
        _front = (_front + 1) % _capacity; 
        _count--;
    }

    T& front() {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty");
        }
        return data[_front];
    }

    bool isEmpty() const { return _count == 0; }
    size_t size() const { return _count; }
};

#endif