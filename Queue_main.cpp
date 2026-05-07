#include<iostream>
#include<string>
#include"Queue.hpp"

int main() {
    Queue<std::string> bankLine;

    std::cout << " Testing Circular Queue (FIFO) " << std::endl;

    bankLine.enqueue("Customer 1: John");
    bankLine.enqueue("Customer 2: Sarah");
    bankLine.enqueue("Customer 3: Mike");

    std::cout << "Queue Size: " << bankLine.size() << std::endl;
    std::cout << "Currently serving: " << bankLine.front() << std::endl;

    std::cout << "\n Serving Customers " << std::endl;
    while (!bankLine.isEmpty()) {
        std::cout << "Finished serving: " << bankLine.front() << std::endl;
        bankLine.dequeue();
        
        if (!bankLine.isEmpty()) {
            std::cout << "Next in line: " << bankLine.front() << std::endl;
        }
    }

    Queue<int> q;
    for(int i = 1; i <= 5; i++) q.enqueue(i);
    
    std::cout << "\nQueue of ints front: " << q.front() << " (Size: " << q.size() << ")" << std::endl;

    return 0;
}