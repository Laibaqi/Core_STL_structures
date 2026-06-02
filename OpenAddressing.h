#pragma once
#include<iostream>
#include<stdexcept>
#include<utility>
#include<functional>

enum ProbingStrategy { LINEAR, QUADRATIC, DOUBLE_HASH };

template <typename Key, typename Value>
class OpenAddressingHashMap {
private:
    enum SlotStatus { EMPTY, OCCUPIED, DELETED };

    struct Entry {
        Key key;
        Value value;
        SlotStatus status = EMPTY;
    };

    Entry* table;          
    size_t capacity;       
    size_t total_elements; 
    ProbingStrategy strategy;

    size_t primaryHash(const Key& key) const {
        std::hash<Key> internalHasher; 
        return internalHasher(key) % capacity;
    }

    size_t secondaryHash(const Key& key) const {
        std::hash<Key> internalHasher;
        size_t rawHash = internalHasher(key);
        size_t prime = (capacity <= 7) ? 3 : 7;
        return prime - (rawHash % prime);
    }

    size_t getProbedIndex(const Key& key, size_t base_index, size_t step) const {
        if (strategy == LINEAR) {
            return (base_index + step) % capacity;
        } 
        else if (strategy == QUADRATIC) {
            return (base_index + (step * step)) % capacity;
        } 
        else { 
            return (base_index + step * secondaryHash(key)) % capacity;
        }
    }

    void rehash() {
        size_t old_capacity = capacity;
        Entry* old_table = table;

        capacity *= 2;
        table = new Entry[capacity]; 
        total_elements = 0;

        for (size_t i = 0; i < old_capacity; ++i) {
            if (old_table[i].status == OCCUPIED) {
                insert_or_update(old_table[i].key, old_table[i].value);
            }
        }
        delete[] old_table; 
    }

    void insert_or_update(const Key& key, const Value& value) {
        if ((float)total_elements / capacity >= 0.55f) {
            rehash();
        }

        size_t base_idx = primaryHash(key);
        int first_deleted_idx = -1;

        for (size_t i = 0; i < capacity; ++i) {
            size_t probe_idx = getProbedIndex(key, base_idx, i);

            if (table[probe_idx].status == DELETED && first_deleted_idx == -1) {
                first_deleted_idx = static_cast<int>(probe_idx);
            }

            if (table[probe_idx].status == EMPTY) {
                size_t insert_target = (first_deleted_idx != -1) ? first_deleted_idx : probe_idx;
                table[insert_target].key = key;
                table[insert_target].value = value;
                table[insert_target].status = OCCUPIED;
                total_elements++;
                return;
            }

            if (table[probe_idx].status == OCCUPIED && table[probe_idx].key == key) {
                table[probe_idx].value = value;
                return;
            }
        }
    }

public:
    class Iterator {
    private:
        Entry* current_ptr;
        Entry* end_ptr;

        void advance_to_valid() {
            while (current_ptr != end_ptr && current_ptr->status != OCCUPIED) {
                current_ptr++;
            }
        }

    public:
        Iterator(Entry* start, Entry* finish) : current_ptr(start), end_ptr(finish) {
            if (current_ptr != end_ptr && current_ptr->status != OCCUPIED) {
                advance_to_valid();
            }
        }

        std::pair<const Key, Value&> operator*() {
            return {current_ptr->key, current_ptr->value};
        }

        Iterator& operator++() {
            if (current_ptr != end_ptr) {
                current_ptr++;
                advance_to_valid();
            }
            return *this;
        }

        bool operator==(const Iterator& other) const {
            return current_ptr == other.current_ptr;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };

    Iterator begin() {
        return Iterator(table, table + capacity);
    }

    Iterator end() {
        return Iterator(table + capacity, table + capacity);
    }

    OpenAddressingHashMap(size_t initial_capacity = 11, ProbingStrategy strat = LINEAR) 
        : capacity(initial_capacity), total_elements(0), strategy(strat) {
        table = new Entry[capacity];
    }

    ~OpenAddressingHashMap() {
        delete[] table;
    }

    OpenAddressingHashMap(const OpenAddressingHashMap& other) {
        capacity = other.capacity;
        strategy = other.strategy;
        total_elements = 0;
        table = new Entry[capacity];

        for (size_t i = 0; i < capacity; ++i) {
            if (other.table[i].status == OCCUPIED) {
                insert_or_update(other.table[i].key, other.table[i].value);
            }
        }
    }

    OpenAddressingHashMap& operator=(const OpenAddressingHashMap& other) {
        if (this != &other) {
            delete[] table;

            capacity = other.capacity;
            strategy = other.strategy;
            total_elements = 0;
            table = new Entry[capacity];

            for (size_t i = 0; i < capacity; ++i) {
                if (other.table[i].status == OCCUPIED) {
                    insert_or_update(other.table[i].key, other.table[i].value);
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
        size_t base_idx = primaryHash(key);
        for (size_t i = 0; i < capacity; ++i) {
            size_t probe_idx = getProbedIndex(key, base_idx, i);
            if (table[probe_idx].status == OCCUPIED && table[probe_idx].key == key) {
                return table[probe_idx].value;
            }
            if (table[probe_idx].status == EMPTY) break;
        }
        insert_or_update(key, Value());
        return at(key);
    }

    Value& at(const Key& key) {
        size_t base_idx = primaryHash(key);
        for (size_t i = 0; i < capacity; ++i) {
            size_t probe_idx = getProbedIndex(key, base_idx, i);
            if (table[probe_idx].status == EMPTY) break;
            if (table[probe_idx].status == OCCUPIED && table[probe_idx].key == key) {
                return table[probe_idx].value;
            }
        }
        throw std::out_of_range("HashMap Error: Key out of bounds.");
    }

    bool erase(const Key& key) {
        size_t base_idx = primaryHash(key);
        for (size_t i = 0; i < capacity; ++i) {
            size_t probe_idx = getProbedIndex(key, base_idx, i);
            if (table[probe_idx].status == EMPTY) return false;
            if (table[probe_idx].status == OCCUPIED && table[probe_idx].key == key) {
                table[probe_idx].status = DELETED; 
                total_elements--;
                return true;
            }
        }
        return false;
    }
};