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

    Node* head; 
    Node* tail;  
    int _size;

    template <typename Compare>
    static Node* mergeSort(Node* node, Compare cmp) {

        if (!node || !node->next) return node;

        Node* slow = node;
        Node* fast = node->next;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        Node* second = slow->next;
        slow->next = nullptr;   
        
        Node* left  = mergeSort(node, cmp);
        Node* right = mergeSort(second, cmp);
        return merge(left, right, cmp);
    }

    template <typename Compare>
    static Node* merge(Node* a, Node* b, Compare cmp) {
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
        tail_ptr->next = (a ? a : b);
        return dummy.next;
    }

public:
    class Iterator {
    private:
        Node* current;
        friend class DoublyLinkedList;
    public:
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

    DoublyLinkedList() : _size(0) {
        head = new Node();
        tail = new Node();
        head->next = tail;
        tail->prev = head;
    }

    DoublyLinkedList(const DoublyLinkedList& other) : _size(0) {
        head = new Node();
        tail = new Node();
        head->next = tail;
        tail->prev = head;
        for (Node* curr = other.head->next; curr != other.tail; curr = curr->next) {
            push_back(curr->data);
        }
    }

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

    Iterator find(const T& val) const {
        for (Node* curr = head->next; curr != tail; curr = curr->next) {
            if (curr->data == val) return Iterator(curr);
        }
        return end();
    }

    int  size()  const { return _size; }
    bool empty() const { return _size == 0; }

    void sort() {
        sort([](const T& a, const T& b) { return a < b; });
    }

    template <typename Compare>
    void sort(Compare cmp) {
        if (_size < 2) return;
        
        Node* chain = head->next;
        tail->prev->next = nullptr;  
        head->next = tail;           
        tail->prev = head;

        chain = mergeSort(chain, cmp);

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

#endif
