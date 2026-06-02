#pragma once
#include<iostream>
#include<stdexcept>
#include<utility>
#include<functional>

template <typename Key, typename Value>
class ChainingHashMap {
public:
    struct Node {
        Key key;
        Value value;
        Node* next;
        Node(const Key& k, const Value& v) : key(k), value(v), next(nullptr) {}
    };

private:
    Node** table;          
    size_t capacity;       
    size_t total_elements; 
    const float MAX_LOAD_FACTOR = 0.75f;

    size_t getHashIndex(const Key& key) const {
        std::hash<Key> internalHasher;
        return internalHasher(key) % capacity;
    }

    void rehash() {
        size_t old_capacity = capacity;
        Node** old_table = table;

        capacity *= 2;
        table = new Node*[capacity](); 
        total_elements = 0;

        for (size_t i = 0; i < old_capacity; ++i) {
            Node* current = old_table[i];
            while (current != nullptr) {
                Node* nextNode = current->next; 
                insert_or_update(current->key, current->value);
                delete current; 
                current = nextNode;
            }
        }
        delete[] old_table; 
    }

    void insert_or_update(const Key& key, const Value& value) {
        if ((float)total_elements / capacity >= MAX_LOAD_FACTOR) {
            rehash();
        }

        size_t index = getHashIndex(key);
        Node* current = table[index];

        while (current != nullptr) {
            if (current->key == key) {
                current->value = value; 
                return;
            }
            current = current->next;
        }

        Node* newNode = new Node(key, value);
        newNode->next = table[index];
        table[index] = newNode;
        total_elements++;
    }

public:
    class Iterator {
    private:
        Node** current_bucket;
        Node** end_bucket;
        Node* current_node;

        void advance_to_valid() {
            while (current_node == nullptr) {
                current_bucket++;
                if (current_bucket == end_bucket) {
                    current_node = nullptr;
                    break;
                }
                current_node = *current_bucket;
            }
        }

    public:
        Iterator(Node** bucket, Node** end, Node* node) 
            : current_bucket(bucket), end_bucket(end), current_node(node) {
            if (current_node == nullptr && current_bucket != end_bucket) {
                advance_to_valid();
            }
        }

        std::pair<const Key, Value&> operator*() {
            return {current_node->key, current_node->value};
        }

        Iterator& operator++() {
            if (current_node != nullptr) {
                current_node = current_node->next;
            }
            if (current_node == nullptr) {
                advance_to_valid();
            }
            return *this;
        }

        bool operator==(const Iterator& other) const {
            return current_node == other.current_node && current_bucket == other.current_bucket;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };

    Iterator begin() {
        return Iterator(table, table + capacity, table[0]);
    }

    Iterator end() {
        return Iterator(table + capacity, table + capacity, nullptr);
    }

    ChainingHashMap(size_t initial_capacity = 8) 
        : capacity(initial_capacity), total_elements(0) {
        table = new Node*[capacity](); 
    }

    ~ChainingHashMap() {
        clear();
        delete[] table;
    }

    ChainingHashMap(const ChainingHashMap& other) {
        capacity = other.capacity;
        total_elements = 0;
        table = new Node*[capacity]();

        for (size_t i = 0; i < capacity; ++i) {
            Node* current = other.table[i];
            while (current != nullptr) {
                insert_or_update(current->key, current->value);
                current = current->next;
            }
        }
    }

    ChainingHashMap& operator=(const ChainingHashMap& other) {
        if (this != &other) {
            clear();
            delete[] table;

            capacity = other.capacity;
            total_elements = 0;
            table = new Node*[capacity]();

            for (size_t i = 0; i < capacity; ++i) {
                Node* current = other.table[i];
                while (current != nullptr) {
                    insert_or_update(current->key, current->value);
                    current = current->next;
                }
            }
        }
        return *this;
    }

    size_t size() const { return total_elements; }
    bool empty() const { return total_elements == 0; }
    float load_factor() const { return (float)total_elements / capacity; }

    void insert(const Key& key, const Value& value) {
        insert_or_update(key, value);
    }

    Value& operator[](const Key& key) {
        size_t index = getHashIndex(key);
        Node* current = table[index];

        while (current != nullptr) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }

        insert_or_update(key, Value());
        index = getHashIndex(key);
        return table[index]->value; 
    }

    Value& at(const Key& key) {
        size_t index = getHashIndex(key);
        Node* current = table[index];

        while (current != nullptr) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }
        throw std::out_of_range("HashMap Error: Key out of bounds.");
    }

    bool erase(const Key& key) {
        size_t index = getHashIndex(key);
        Node* current = table[index];
        Node* prev = nullptr;

        while (current != nullptr) {
            if (current->key == key) {
                if (prev == nullptr) {
                    table[index] = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                total_elements--;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }

    void clear() {
        for (size_t i = 0; i < capacity; ++i) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* nextNode = current->next;
                delete current;
                current = nextNode;
            }
            table[i] = nullptr;
        }
        total_elements = 0;
    }
};