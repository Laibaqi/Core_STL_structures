#ifndef STACK_HPP
#define STACK_HPP

#include <stdexcept>
#include <cstddef>

template <typename T>
class Stack {
private:
    T* data;           // The array pointer
    size_t _top;       // Index of the next available slot (also represents size)
    size_t _capacity;  // Total allocated space

    // Helper to resize the stack when it's full
    void resize() {
        _capacity = (_capacity == 0) ? 1 : _capacity * 2;
        T* newData = new T[_capacity];

        // Copy old elements
        for (size_t i = 0; i < _top; i++) {
            newData[i] = data[i];
        }

        delete[] data;
        data = newData;
    }

public:
    // Constructor
    Stack() : data(nullptr), _top(0), _capacity(0) {}

    // Destructor
    ~Stack() {
        delete[] data;
    }

    // Push: Add to the top
    void push(const T& value) {
        if (_top >= _capacity) {
            resize();
        }
        data[_top] = value;
        _top++;
    }

    // Pop: Remove from the top
    void pop() {
        if (isEmpty()) {
            throw std::out_of_range("Stack Underflow: Cannot pop from empty stack.");
        }
        _top--; // We don't delete the data, we just move the "top" marker down
    }

    // Top: Look at the top element
    T& top() {
        if (isEmpty()) {
            throw std::out_of_range("Stack is empty.");
        }
        return data[_top - 1];
    }

    // Utility functions
    bool isEmpty() const {
        return _top == 0;
    }

    size_t size() const {
        return _top;
    }

    size_t capacity() const {
        return _capacity;
    }
};

#endif