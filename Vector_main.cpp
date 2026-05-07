#include<iostream>
#include"Vector.hpp"

int main() {
    Vector<int> v;

    for(int i = 1; i <= 10; i++) {
        v.push_back(i * 5);
        std::cout << "Added " << i*5 << " | Size: " << v.getSize() 
                  << " | Capacity: " << v.getCapacity() << std::endl;
    }

    std::cout << "\nElement at index 4: " << v[4] << std::endl;

    try {
        std::cout << v[20]; 
    } catch (const std::out_of_range& e) {
        std::cout << "Caught expected error: " << e.what() << std::endl;
    }

    return 0;
}