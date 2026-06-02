#include<iostream>
#include<string>
#include"BST.h"

int main() {
    BinarySearchTree<int, std::string> bst;

    bst.insert(50, "Root");
    bst.insert(30, "Left Child");
    bst.insert(70, "Right Child");
    bst.insert(20, "Leaf Left");
    bst.insert(40, "Leaf Right");

    std::cout << "Tree Size: " << bst.size() << "\n";
    std::cout << "Inorder Traversal: ";
    bst.printInorder();

    bst[40] = "Updated Leaf Right";
    bst[60] = "New Implicit Node";

    std::cout << "\nAfter bracket operators:\n";
    bst.printInorder();

    std::cout << "\nErasing node 30...\n";
    bst.erase(30);
    bst.printInorder();

    try {
        std::cout << "\nFinding key 50: " << bst.at(50) << "\n";
        std::cout << "Finding key 999: " << bst.at(999) << "\n";
    } 
    catch (const std::out_of_range& e) {
        std::cout << "Caught Expected Error: " << e.what() << "\n";
    }

    BinarySearchTree<int, std::string> copyTree = bst;
    std::cout << "\nCopied Tree Content: ";
    copyTree.printInorder();

    return 0;
}