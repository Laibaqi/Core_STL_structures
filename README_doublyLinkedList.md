# DoublyLinkedList

A header-only, STL-style doubly linked list implemented from scratch in C++. Part of a Data Structures and Algorithms course project that reimplements core STL containers without using the standard library's versions.

## Overview

This implementation provides a generic (templated) doubly linked list with sentinel nodes, a bidirectional iterator, full Rule-of-Three compliance, and an in-place merge sort. The design prioritizes clean code through the sentinel pattern, which eliminates special cases for empty lists and boundary operations.

## Features

- **Generic** — works with any type via C++ templates
- **Header-only** — drop `DoublyLinkedList.h` into any project and `#include` it
- **Sentinel-based** — dummy head and tail nodes simplify all insert/delete logic
- **STL-style iterator** — supports range-based for loops and STL algorithm compatibility
- **Rule of Three compliant** — proper deep copy semantics
- **In-place merge sort** — O(n log n) sort with no data copies, just pointer rewiring
- **Exception-safe boundaries** — throws `std::out_of_range` on invalid access

## File Structure

```
.
├── DoublyLinkedList.h    Header-only library (include this)
├── test.cpp              Correctness tests + runtime benchmarks
└── README.md             This file
```

## Build and Run

Requires a C++17-compatible compiler (g++, clang++, or MSVC).

```bash
g++ -std=c++17 -O2 -Wall -o test test.cpp
./test
```

The test binary runs 12 labeled correctness tests followed by runtime benchmarks at N = 1000, 5000, and 10000 elements.

## Usage

```cpp
#include "DoublyLinkedList.h"

DoublyLinkedList<int> list;

list.push_back(10);
list.push_back(20);
list.push_front(5);
list.insert_at(1, 7);          // list: 5 <-> 7 <-> 10 <-> 20

// Range-based for loop via iterator
for (auto x : list) {
    std::cout << x << " ";
}

// Sort with default operator<
list.sort();

// Sort with custom comparator (descending)
list.sort([](int a, int b) { return a > b; });

// Deep copy is safe
DoublyLinkedList<int> copy = list;
```

## API Reference

### Constructors and Destructor
| Method | Description | Complexity |
|--------|-------------|------------|
| `DoublyLinkedList()` | Default constructor (empty list with sentinels) | O(1) |
| `DoublyLinkedList(const DoublyLinkedList&)` | Deep copy constructor | O(n) |
| `operator=(const DoublyLinkedList&)` | Deep copy assignment | O(n) |
| `~DoublyLinkedList()` | Destroys all nodes | O(n) |

### Modifiers
| Method | Description | Complexity |
|--------|-------------|------------|
| `push_back(const T&)` | Insert at end | O(1) |
| `push_front(const T&)` | Insert at beginning | O(1) |
| `pop_back()` | Remove last element | O(1) |
| `pop_front()` | Remove first element | O(1) |
| `insert_at(int, const T&)` | Insert at index | O(n) |
| `remove(const T&)` | Remove first occurrence; returns `bool` | O(n) |
| `clear()` | Remove all elements | O(n) |
| `sort()` | In-place merge sort (ascending) | O(n log n) |
| `sort(Compare)` | In-place merge sort with custom comparator | O(n log n) |

### Accessors
| Method | Description | Complexity |
|--------|-------------|------------|
| `front()` | First element (throws if empty) | O(1) |
| `back()` | Last element (throws if empty) | O(1) |
| `find(const T&)` | Iterator to first match, or `end()` | O(n) |
| `size()` | Number of elements | O(1) |
| `empty()` | Whether list has no elements | O(1) |
| `begin() / end()` | Iterators for traversal | O(1) |
| `display()` | Print contents to stdout | O(n) |

## Design Decisions

### Sentinel Nodes
The list maintains two permanent dummy nodes (head and tail sentinels) that don't hold real data. The first real element sits at `head->next` and the last at `tail->prev`. This eliminates special cases throughout the codebase — there is never an "empty list" edge case for insertion or deletion, because every real node always has valid neighbors on both sides.

**Trade-off:** Two extra nodes of memory per list, regardless of size. Negligible for any practical use case, and the simplification of insert/delete logic is substantial.

### Bidirectional Iterator
The custom `Iterator` class implements the five STL iterator traits (`iterator_category`, `value_type`, `difference_type`, `pointer`, `reference`) tagged as `std::bidirectional_iterator_tag`. This means:

- Range-based for loops work: `for (auto x : list) { ... }`
- STL algorithms like `std::find`, `std::distance`, `std::copy` are compatible
- Both pre-increment (`++it`) and post-increment (`it++`) are supported, along with their decrement counterparts

### Rule of Three
The class manages raw memory through `new` and `delete`, so it implements all three special member functions: destructor, copy constructor, and copy assignment operator. Without the copy operations, a default shallow copy would cause double-free crashes when both objects went out of scope. The copy assignment includes a self-assignment guard (`if (this == &other) return *this;`) to prevent destroying data we're about to read from.

### Insert-at Optimization
`insert_at(index, val)` chooses to walk forward from `head` or backward from `tail` based on which is closer to the target index. Still O(n) asymptotically, but with a 2× constant-factor improvement over always traversing from one end.

### Merge Sort Strategy
The sort algorithm operates in three phases:

1. **Detach** real nodes from sentinels into a singly-linked chain (ignoring `prev` pointers)
2. **Recursively split and merge** using the classic slow/fast pointer technique to find midpoints
3. **Reattach** the sorted chain to sentinels, rebuilding all `prev` pointers in a single pass

**Why merge sort:** Quicksort relies on random access for efficient partitioning, which is O(k) on linked lists. Merge sort only needs sequential access, which linked lists provide natively. This is also why `std::list::sort` uses merge sort.

**Properties:**
- **Time:** O(n log n) — guaranteed, no worst-case degradation
- **Space:** O(log n) — recursion stack only; no auxiliary arrays
- **Stable:** equal elements preserve relative order
- **Zero data copies:** only pointer rewiring, so sorting large objects is cheap

## Benchmark Results

Measured on N = 1000, 5000, and 10000 elements. All times in microseconds.

| Operation | N=1000 | N=5000 | N=10000 | Complexity |
|-----------|--------|--------|---------|------------|
| push_back × N | 21 | 95 | 71 | O(1) per call |
| push_front × N | 44 | 102 | 324 | O(1) per call |
| insert_at(mid) × N/10 | 77 | 3107 | 14074 | O(n) per call |
| find (worst case) | 0 | 0 | 0 | O(n) |
| iterate all | 0 | 0 | 0 | O(n) |
| remove × N/10 | 0 | 5 | 11 | O(n) per call |
| pop_front × N/2 | 4 | 21 | 43 | O(1) per call |
| pop_back × N/2 | 4 | 19 | 39 | O(1) per call |
| **sort (reverse data)** | **23** | **147** | **349** | **O(n log n)** |

The merge sort timings empirically confirm O(n log n) complexity: a quadratic algorithm would take ~100× longer at N=10000 vs N=1000 (about 2300 µs), but actual growth is only ~15×, matching the predicted ratio of `(10000 × log 10000) / (1000 × log 1000) ≈ 13.3`.

The `insert_at` timings empirically confirm its O(n) per-call cost: total time for N/10 inserts grows roughly quadratically (77 → 3107 → 14074), which is exactly what O(n²) total work looks like.

## Testing

The test binary runs the following correctness checks before benchmarking:

1. `push_back` and `push_front` produce the expected ordering
2. `insert_at` places elements at the requested index
3. `remove` removes the first matching element
4. `front` and `back` return correct boundary elements
5. `pop_front` and `pop_back` remove from the correct ends
6. Iterator supports range-based for loops
7. `find` returns valid iterator on hit, `end()` on miss
8. Copy constructor produces an independent deep copy
9. `clear()` resets the list to empty state
10. `sort()` correctly sorts a scrambled list ascending
11. `sort()` with custom comparator sorts descending
12. `sort()` handles edge cases (empty list, single element)

All tests print expected vs actual output for visual verification.

## Limitations

- **Not thread-safe.** Concurrent access requires external synchronization.
- **No move semantics.** Move constructor and move assignment are not implemented (would be a Rule-of-Five extension). For this assignment's scope this is acceptable; the copy versions handle all required cases correctly.
- **No allocator support.** Uses `new` and `delete` directly rather than a custom allocator parameter like `std::list` does.

## Project Context

This implementation is part of a Data Structures and Algorithms course project requiring reimplementation of core STL containers. The project specification asked for header-file usability, which this design provides — the entire library is contained in `DoublyLinkedList.h` and requires no separate compilation unit.

## Author

Group project for DSA coursework.
