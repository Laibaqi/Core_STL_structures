#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>    
#include <stdexcept>  

template <typename T>
class Vector {
private:
    T* data;           
    size_t size;       
    size_t capacity;   

    void grow() {
        size_t newCapacity = (capacity == 0) ? 1 : capacity * 2;
        T* newData = new T[newCapacity];

        for (size_t i = 0; i < size; i++) {
            newData[i] = data[i];
        }

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    Vector() {
        data = nullptr;
        size = 0;
        capacity = 0;
    }

    ~Vector() {
        delete[] data;
    }

    Vector(const Vector& other) {
        size = other.size;
        capacity = other.capacity;
        
        if (capacity > 0) {
            data = new T[capacity];
            for (size_t i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        } else {
            data = nullptr;
        }
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] data;

            size = other.size;
            capacity = other.capacity;

            if (capacity > 0) {
                data = new T[capacity];
                for (size_t i = 0; i < size; i++) {
                    data[i] = other.data[i];
                }
            } else {
                data = nullptr;
            }
        }
        return *this;
    }

    void push_back(const T& value) {
        if (size >= capacity) {
            grow();
        }
        data[size] = value;
        size++;
    }

    void pop_back() {
        if (size > 0) {
            size--;
        }
    }

    void clear() {
        size = 0;
    }

    T& operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    size_t getSize() const { return size; }
    size_t getCapacity() const { return capacity; }
    bool isEmpty() const { return size == 0; }
};

#endif