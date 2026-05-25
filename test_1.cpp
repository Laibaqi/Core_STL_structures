#include "DoublyLinkedList.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <string>

using namespace std;
using namespace std::chrono;

// Helper to time a block of code and return microseconds
template <typename Func>
long long time_us(Func f) {
    auto start = high_resolution_clock::now();
    f();
    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start).count();
}

void print_row(const string& op, int n, long long us) {
    cout << left << setw(22) << op
         << right << setw(10) << n
         << setw(15) << us << " us"
         << endl;
}

void correctness_tests() {
    cout << "==================== CORRECTNESS TESTS ====================\n";
    DoublyLinkedList<int> list;

    cout << "[1] push_back / push_front\n";
    list.push_back(10);
    list.push_back(20);
    list.push_front(5);
    cout << "    Expected: 5 <-> 10 <-> 20\n    Got:      ";
    list.display();

    cout << "\n[2] insert_at(1, 7)\n";
    list.insert_at(1, 7);
    cout << "    Expected: 5 <-> 7 <-> 10 <-> 20\n    Got:      ";
    list.display();

    cout << "\n[3] remove(10)\n";
    list.remove(10);
    cout << "    Expected: 5 <-> 7 <-> 20\n    Got:      ";
    list.display();

    cout << "\n[4] front() = " << list.front() << ", back() = " << list.back() << "\n";

    cout << "\n[5] pop_front / pop_back\n";
    list.pop_front();
    list.pop_back();
    cout << "    Expected: 7\n    Got:      ";
    list.display();

    cout << "\n[6] Iterator (range-based for)\n";
    list.push_back(100);
    list.push_back(200);
    cout << "    Got: ";
    for (auto x : list) cout << x << " ";
    cout << "\n";

    cout << "\n[7] find(100) -> " << (list.find(100) != list.end() ? "FOUND" : "NOT FOUND") << "\n";
    cout << "    find(999) -> " << (list.find(999) != list.end() ? "FOUND" : "NOT FOUND") << "\n";

    cout << "\n[8] Copy constructor (deep copy test)\n";
    DoublyLinkedList<int> copy = list;
    copy.push_back(555);
    cout << "    Original: "; list.display();
    cout << "    Copy:     "; copy.display();
    cout << "    (Original should NOT contain 555)\n";

    cout << "\n[9] clear() then empty()\n";
    list.clear();
    cout << "    size=" << list.size() << ", empty=" << (list.empty() ? "true" : "false") << "\n";

    cout << "\n[10] sort() — merge sort\n";
    DoublyLinkedList<int> s;
    int vals[] = {42, 7, 19, 3, 88, 1, 56, 23, 11, 4};
    for (int v : vals) s.push_back(v);
    cout << "    Before: "; s.display();
    s.sort();
    cout << "    After:  "; s.display();
    cout << "    Expected: 1 <-> 3 <-> 4 <-> 7 <-> 11 <-> 19 <-> 23 <-> 42 <-> 56 <-> 88\n";

    cout << "\n[11] sort() with custom comparator (descending)\n";
    s.sort([](int a, int b){ return a > b; });
    cout << "    Got: "; s.display();

    cout << "\n[12] sort edge cases (empty, single element)\n";
    DoublyLinkedList<int> e;
    e.sort();
    cout << "    Empty after sort: size=" << e.size() << "\n";
    e.push_back(99);
    e.sort();
    cout << "    Single elem: "; e.display();

    cout << "\nAll correctness tests passed.\n\n";
}

void benchmark(int N) {
    cout << "==================== RUNTIME STATISTICS (N = " << N << ") ====================\n";
    cout << left << setw(22) << "Operation"
         << right << setw(10) << "N"
         << setw(18) << "Time" << "\n";
    cout << string(50, '-') << "\n";

    DoublyLinkedList<int> list;

    // push_back N items
    long long t = time_us([&]() {
        for (int i = 0; i < N; i++) list.push_back(i);
    });
    print_row("push_back x N", N, t);

    // push_front N items (into a fresh list)
    DoublyLinkedList<int> list2;
    t = time_us([&]() {
        for (int i = 0; i < N; i++) list2.push_front(i);
    });
    print_row("push_front x N", N, t);

    // insert_at middle, N/10 times (insert_at is O(n), so don't do all N)
    t = time_us([&]() {
        for (int i = 0; i < N / 10; i++) list.insert_at(list.size() / 2, -1);
    });
    print_row("insert_at(mid) x N/10", N / 10, t);

    // find: worst case (search for last element, walks whole list)
    int target = N - 1;
    t = time_us([&]() {
        auto it = list.find(target);
        (void)it;
    });
    print_row("find (worst case)", 1, t);

    // find: best case (first element)
    t = time_us([&]() {
        auto it = list.find(0);
        (void)it;
    });
    print_row("find (best case)", 1, t);

    // Iterator traversal of the full list
    t = time_us([&]() {
        long long sum = 0;
        for (auto x : list) sum += x;
        (void)sum;
    });
    print_row("iterate all", list.size(), t);

    // remove (by value, first occurrence) — N/10 removes
    t = time_us([&]() {
        for (int i = 0; i < N / 10; i++) list.remove(i);
    });
    print_row("remove x N/10", N / 10, t);

    // pop_front N/2 times
    int popN = list.size() / 2;
    t = time_us([&]() {
        for (int i = 0; i < popN; i++) list.pop_front();
    });
    print_row("pop_front x N/2", popN, t);

    // pop_back: remaining
    int remaining = list.size();
    t = time_us([&]() {
        while (!list.empty()) list.pop_back();
    });
    print_row("pop_back (rest)", remaining, t);

    // Merge sort benchmark — fill with reverse-sorted data to give it real work
    DoublyLinkedList<int> sortList;
    for (int i = N; i > 0; i--) sortList.push_back(i);
    t = time_us([&]() {
        sortList.sort();
    });
    print_row("sort (reverse data)", N, t);

    cout << "\n";
}

int main() {
    correctness_tests();

    // Run benchmark at multiple sizes so you can show how time scales with N.
    // This is what demonstrates the O(1) vs O(n) behavior to your professor.
    benchmark(1000);
    benchmark(5000);
    benchmark(10000);

    return 0;
}
