# Core STL Structures

Seven C++ containers modeled on the standard library, built from raw pointers with manual memory management and no `std::` containers underneath.

## The containers

| Container | File | How it works |
|-----------|------|--------------|
| `Vector<T>` | `Vector.hpp` | Dynamic array that doubles its capacity when full; deep-copy constructor and assignment; bounds-checked `operator[]` |
| `Stack<T>` | `Stack.hpp` | Array-backed LIFO with amortized O(1) push and underflow protection |
| `Queue<T>` | `Queue.hpp` | **Circular buffer** FIFO: front and rear indices wrap around, and the buffer is unrolled in order on resize |
| `DoublyLinkedList<T>` | `DoublyLinkedList.h` | Head/tail pointers, insert at any index, bidirectional iterator for range-based `for`, and in-place **merge sort** with a custom comparator |
| `BinarySearchTree<K, V>` | `BST.h` | Key-value map with recursive insert, find and erase (in-order successor), `operator[]` and deep copy |
| `ChainingHashMap<K, V>` | `ChainingHashMap.h` | Separate chaining with linked buckets, automatic rehashing by load factor and an iterator |
| `OpenAddressingHashMap<K, V>` | `OpenAddressing.h` | Open addressing with a choice of **linear, quadratic or double hashing**; **tombstone deletion** keeps probe chains intact, and inserts reuse the first tombstone |

## Highlights

- **Templates throughout**: every container works with any type, from `int` to `std::string`.
- **Iterators**: the linked list and both hash maps support range-based `for` loops.
- **Safety**: out-of-range access and empty pops throw exceptions instead of failing silently.
- **Benchmark**: `stack_comparison.cpp` times the custom `Stack` against `std::stack` over 5,000 push and pop operations.

## Build and run

Each container has its own demo program. You need a C++17 compiler:

```bash
g++ -std=c++17 Vector_main.cpp -o vector && ./vector
g++ -std=c++17 Stack_main.cpp -o stack && ./stack
g++ -std=c++17 Queue_main.cpp -o queue && ./queue
g++ -std=c++17 doublyLinkedListMain.cpp -o list && ./list
g++ -std=c++17 BST_main.cpp -o bst && ./bst
g++ -std=c++17 Hashing_main.cpp -o hashing && ./hashing
g++ -std=c++17 -O2 stack_comparison.cpp -o bench && ./bench
```

On Windows, run the program with `vector.exe` instead of `./vector`.

## Example output

```
TESTING OPEN ADDRESSING MAP (DOUBLE HASHING)
Active Map Content after deletion via Iterator Range Loop:
  Subject: Physics -> Score: 98
  Subject: Biology -> Score: 93

Attempting access to dropped subject 'Chemistry' via .at():
Exception Caught -> HashMap Error: Key out of bounds.
```
