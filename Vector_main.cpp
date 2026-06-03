#include <iostream>
#include "Vector.hpp"

int main() {
    std::cout << "Creating Vector v1 and pushing elements" << std::endl;
    Vector<int> v1;
    
    v1.push_back(10);
    v1.push_back(20);
    v1.push_back(30);

    std::cout << "v1 Size: " << v1.getSize() << std::endl;
    std::cout << "v1 Capacity: " << v1.getCapacity() << std::endl;
    std::cout << "v1 Element at index 1: " << v1[1] << std::endl;

    std::cout << "Testing Copy Constructor" << std::endl;
    Vector<int> v2 = v1;
    std::cout << "v2 Size: " << v2.getSize() << std::endl;
    
    v1[0] = 999;
    std::cout << "v1[0] after change: " << v1[0] << std::endl;
    std::cout << "v2[0]: " << v2[0] << std::endl;

    std::cout << "Testing Assignment Operator" << std::endl;
    Vector<int> v3;
    v3 = v1;
    std::cout << "v3[0]: " << v3[0] << std::endl;

    std::cout << "Testing Pop Back and Out of Bounds Protection" << std::endl;
    v1.pop_back();
    std::cout << "v1 Size after pop_back: " << v1.getSize() << std::endl;

    try {
        std::cout << "Attempting to access invalid index v1[5]" << std::endl;
        std::cout << v1[5] << std::endl;
    } catch (const std::out_of_range& e) {
        std::cout << "Caught expected safety error: " << e.what() << std::endl;
    }

    std::cout << "All core functions tested successfully" << std::endl;
    return 0;
}