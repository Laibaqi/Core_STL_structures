#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <iostream>
#include <stdexcept>
#include <iterator>

template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node* prev;
        Node() : data(T()), next(nullptr), prev(nullptr) {}
        Node(const T& val) : data(val), next(nullptr), prev(nullptr) {}
    };

    Node* head;   // sentinel (dummy) head
    Node* tail;   // sentinel (dummy) tail
    int _size;

    // ---------- Sort helpers (private) ----------
    // These operate on a singly-linked chain using only `next` pointers.
    // `prev` is ignored here and rebuilt by sort() after merging completes.

    template <typename Compare>
    static Node* mergeSort(Node* node, Compare cmp) {
        // Base case: 0 or 1 nodes is already sorted.
        if (!node || !node->next) return node;

        // Split the chain in half using slow/fast pointer technique.
        Node* slow = node;
        Node* fast = node->next;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        Node* second = slow->next;
        slow->next = nullptr;   // cut the chain into two halves

        // Recursively sort each half, then merge.
        Node* left  = mergeSort(node, cmp);
        Node* right = mergeSort(second, cmp);
        return merge(left, right, cmp);
    }

    template <typename Compare>
    static Node* merge(Node* a, Node* b, Compare cmp) {
        // Dummy node simplifies the head case — no special-casing the first pick.
        Node dummy;
        Node* tail_ptr = &dummy;

        while (a && b) {
            if (cmp(a->data, b->data)) {
                tail_ptr->next = a;
                a = a->next;
            } else {
                tail_ptr->next = b;
                b = b->next;
            }
            tail_ptr = tail_ptr->next;
        }
        tail_ptr->next = (a ? a : b);   // attach whichever chain still has nodes
        return dummy.next;
    }

public:
    // ---------- Iterator ----------
    // Bidirectional iterator so range-based for loops and STL algorithms work.
    class Iterator {
    private:
        Node* current;
        friend class DoublyLinkedList;
    public:
        // STL iterator traits — lets std::distance, std::advance, etc. work
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

        Iterator(Node* node) : current(node) {}

        T& operator*()  const { return current->data; }
        T* operator->() const { return &(current->data); }

        Iterator& operator++()    { current = current->next; return *this; }     // ++it
        Iterator  operator++(int) { Iterator tmp = *this; current = current->next; return tmp; } // it++
        Iterator& operator--()    { current = current->prev; return *this; }
        Iterator  operator--(int) { Iterator tmp = *this; current = current->prev; return tmp; }

        bool operator==(const Iterator& other) const { return current == other.current; }
        bool operator!=(const Iterator& other) const { return current != other.current; }
    };

    Iterator begin() const { return Iterator(head->next); }
    Iterator end()   const { return Iterator(tail); }

    // ---------- Constructors / Destructor ----------
    DoublyLinkedList() : _size(0) {
        head = new Node();
        tail = new Node();
        head->next = tail;
        tail->prev = head;
    }

    // Copy constructor (deep copy) — Rule of Three
    DoublyLinkedList(const DoublyLinkedList& other) : _size(0) {
        head = new Node();
        tail = new Node();
        head->next = tail;
        tail->prev = head;
        for (Node* curr = other.head->next; curr != other.tail; curr = curr->next) {
            push_back(curr->data);
        }
    }

    // Copy assignment (deep copy) — Rule of Three
    DoublyLinkedList& operator=(const DoublyLinkedList& other) {
        if (this == &other) return *this;
        clear();
        for (Node* curr = other.head->next; curr != other.tail; curr = curr->next) {
            push_back(curr->data);
        }
        return *this;
    }

    ~DoublyLinkedList() {
        Node* current = head;
        while (current) {
            Node* nxt = current->next;
            delete current;
            current = nxt;
        }
    }

    // ---------- Modifiers ----------
    void push_back(const T& val) {
        Node* newNode = new Node(val);
        Node* lastReal = tail->prev;

        newNode->next = tail;
        newNode->prev = lastReal;
        lastReal->next = newNode;
        tail->prev = newNode;
        _size++;
    }

    void push_front(const T& val) {
        Node* newNode = new Node(val);
        Node* firstReal = head->next;

        newNode->prev = head;
        newNode->next = firstReal;
        head->next = newNode;
        firstReal->prev = newNode;
        _size++;
    }

    void pop_back() {
        if (_size == 0) throw std::out_of_range("pop_back on empty list");
        Node* target = tail->prev;
        target->prev->next = tail;
        tail->prev = target->prev;
        delete target;
        _size--;
    }

    void pop_front() {
        if (_size == 0) throw std::out_of_range("pop_front on empty list");
        Node* target = head->next;
        head->next = target->next;
        target->next->prev = head;
        delete target;
        _size--;
    }

    void insert_at(int index, const T& val) {
        if (index < 0 || index > _size) {
            throw std::out_of_range("Index out of bounds");
        }

        // Walk from whichever end is closer — O(n/2) worst case
        Node* curr;
        if (index < _size / 2) {
            curr = head->next;
            for (int i = 0; i < index; i++) curr = curr->next;
        } else {
            curr = tail;
            for (int i = _size; i > index; i--) curr = curr->prev;
        }

        Node* newNode = new Node(val);
        newNode->next = curr;
        newNode->prev = curr->prev;
        curr->prev->next = newNode;
        curr->prev = newNode;
        _size++;
    }

    // Removes the FIRST occurrence of val. Returns true if removed.
    bool remove(const T& val) {
        Node* curr = head->next;
        while (curr != tail) {
            if (curr->data == val) {
                curr->prev->next = curr->next;
                curr->next->prev = curr->prev;
                delete curr;
                _size--;
                return true;
            }
            curr = curr->next;
        }
        return false;
    }

    void clear() {
        Node* curr = head->next;
        while (curr != tail) {
            Node* nxt = curr->next;
            delete curr;
            curr = nxt;
        }
        head->next = tail;
        tail->prev = head;
        _size = 0;
    }

    // ---------- Accessors ----------
    T& front() {
        if (_size == 0) throw std::out_of_range("front() on empty list");
        return head->next->data;
    }

    T& back() {
        if (_size == 0) throw std::out_of_range("back() on empty list");
        return tail->prev->data;
    }

    const T& front() const {
        if (_size == 0) throw std::out_of_range("front() on empty list");
        return head->next->data;
    }

    const T& back() const {
        if (_size == 0) throw std::out_of_range("back() on empty list");
        return tail->prev->data;
    }

    // Returns iterator to first match, or end() if not found
    Iterator find(const T& val) const {
        for (Node* curr = head->next; curr != tail; curr = curr->next) {
            if (curr->data == val) return Iterator(curr);
        }
        return end();
    }

    int  size()  const { return _size; }
    bool empty() const { return _size == 0; }

    // ---------- Sort ----------
    // Merge sort on the linked list: O(n log n) time, O(log n) stack space.
    // Works by rewiring node pointers — no copying of T values, no extra arrays.
    // Uses operator< on T by default; pass a comparator for custom orderings.
    void sort() {
        sort([](const T& a, const T& b) { return a < b; });
    }

    template <typename Compare>
    void sort(Compare cmp) {
        if (_size < 2) return;

        // Step 1: detach real nodes from sentinels into a plain singly-linked
        // chain using only `next` pointers. We'll rebuild `prev` at the end.
        Node* chain = head->next;
        tail->prev->next = nullptr;     // terminate the chain
        head->next = tail;              // temporarily empty the list
        tail->prev = head;

        // Step 2: recursively merge-sort the chain.
        chain = mergeSort(chain, cmp);

        // Step 3: re-attach to sentinels and rebuild prev pointers in one pass.
        Node* prev = head;
        Node* curr = chain;
        while (curr) {
            curr->prev = prev;
            prev->next = curr;
            prev = curr;
            curr = curr->next;
        }
        prev->next = tail;
        tail->prev = prev;
    }

    void display() const {
        Node* curr = head->next;
        while (curr != tail) {
            std::cout << curr->data;
            if (curr->next != tail) std::cout << " <-> ";
            curr = curr->next;
        }
        std::cout << std::endl;
    }
};

#endif // DOUBLY_LINKED_LIST_H
