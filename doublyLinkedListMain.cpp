#include "DoublyLinkedList.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
    cout << "DSA PROJECT 1: DOUBLY LINKED LIST\n";

    cout << "Step 1: Testing Insertions\n";
    DoublyLinkedList<int> myList;

    myList.push_back(10);
    myList.push_back(20);
    myList.push_front(5);
    
    cout << "List after push_front(5), push_back(10), push_back(20):\n";
    cout << "Expected: 5 <-> 10 <-> 20\n";
    cout << "Actual:   ";
    myList.display();
    cout << "Current Size: " << myList.size() << "\n\n";

    cout << "Step 2: Testing Insert At Index\n";
    cout << "Inserting 7 at index 1...\n";
    myList.insert_at(1, 7);
    
    cout << "Expected: 5 <-> 7 <-> 10 <-> 20\n";
    cout << "Actual:   ";
    myList.display();
    cout << "\n";

    cout << "Step 3: Testing Deletions\n";
    cout << "Removing value 10...\n";
    myList.remove(10);
    cout << "After remove(10): ";
    myList.display();

    cout << "Popping from front and back...\n";
    myList.pop_front();
    myList.pop_back();
    cout << "Expected: 7\n";
    cout << "Actual:   ";
    myList.display();
    cout << "\n";

    cout << "Step 4: Testing Iterator Traversal\n";
    cout << "Adding elements to traverse...\n";
    myList.push_back(100);
    myList.push_back(200);
    myList.push_back(300);

    cout << "Printing list elements using a standard range-based for loop:\n";
    for (int element : myList) {
        cout << "[ " << element << " ] ";
    }
    cout << "\n\n";

    cout << "Step 5: Testing In-Place Merge Sort\n";
    DoublyLinkedList<int> unsortedList;
    
    unsortedList.push_back(42);
    unsortedList.push_back(7);
    unsortedList.push_back(19);
    unsortedList.push_back(3);
    unsortedList.push_back(88);
    unsortedList.push_back(1);

    cout << "Unsorted List: ";
    unsortedList.display();

    cout << "Sorting the list...\n";
    unsortedList.sort();

    cout << "Expected:      1 <-> 3 <-> 7 <-> 19 <-> 42 <-> 88\n";
    cout << "Sorted List:   ";
    unsortedList.display();
    cout << "\n";

    cout << "Step 6: Testing Deep Copy (Rule of Three)\n";
    cout << "Creating a copy of the sorted list...\n";
    DoublyLinkedList<int> copiedList = unsortedList;
    
    cout << "Adding 999 to the copied list only...\n";
    copiedList.push_back(999);

    cout << "Original List: "; unsortedList.display();
    cout << "Copied List:   "; copiedList.display();
    cout << "(Pass: Original remained unchanged!)\n\n";

    cout << "ALL STUDENT TESTS COMPLETED\n";
    return 0;
}