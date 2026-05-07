#include<iostream>
#include<string>
#include"Stack.hpp"

int main() {
    Stack<std::string> undoStack;

    std::cout << " Testing Stack with Strings " << std::endl;

    std::cout << "Action: Typing Hello" << std::endl;
    undoStack.push("Hello");
    std::cout << "Action: Typing World" << std::endl;
    undoStack.push("World");
    std::cout << "Action: Adding Exclamation mark" << std::endl;
    undoStack.push("!!!");

    std::cout << "Current Stack Size: " << undoStack.size() << std::endl;
    std::cout << "Top Element: " << undoStack.top() << std::endl;

    std::cout << "\n Performing Undo (Pop) " << std::endl;
    while (!undoStack.isEmpty()) {
        std::cout << "Removing: " << undoStack.top() << std::endl;
        undoStack.pop();
        std::cout << "New Size: " << undoStack.size() << std::endl;
    }

    std::cout << "\n Testing Error Handling " << std::endl;
    try {
        std::cout << "Attempting to pop from an empty stack" << std::endl;
        undoStack.pop();
    } catch (const std::out_of_range& e) {
        std::cout << "Caught Expected Error: " << e.what() << std::endl;
    }

    Stack<int> numbers;
    numbers.push(100);
    numbers.push(200);
    std::cout << "\nInteger Stack Top: " << numbers.top() << std::endl;

    return 0;
}