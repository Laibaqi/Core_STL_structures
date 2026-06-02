#pragma once
#include<iostream>
#include<stdexcept>
#include<utility>

template <typename Key, typename Value>
class BinarySearchTree {
public:
    struct Node {
        Key key;
        Value value;
        Node* left;
        Node* right;
        Node(const Key& k, const Value& v) : key(k), value(v), left(nullptr), right(nullptr) {}
    };

private:
    Node* root;
    size_t total_nodes;

    void destroyTree(Node* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    Node* copyTree(Node* node) {
        if (node == nullptr) return nullptr;
        Node* newNode = new Node(node->key, node->value);
        newNode->left = copyTree(node->left);
        newNode->right = copyTree(node->right);
        return newNode;
    }

    Node* insertHelper(Node* node, const Key& key, const Value& value, bool& inserted) {
        if (node == nullptr) {
            inserted = true;
            return new Node(key, value);
        }
        if (key < node->key) {
            node->left = insertHelper(node->left, key, value, inserted);
        } else if (key > node->key) {
            node->right = insertHelper(node->right, key, value, inserted);
        } else {
            node->value = value;
            inserted = false;
        }
        return node;
    }

    Node* findMin(Node* node) const {
        while (node && node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    Node* eraseHelper(Node* node, const Key& key, bool& removed) {
        if (node == nullptr) {
            removed = false;
            return nullptr;
        }
        if (key < node->key) {
            node->left = eraseHelper(node->left, key, removed);
        } else if (key > node->key) {
            node->right = eraseHelper(node->right, key, removed);
        } else {
            removed = true;
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            Node* temp = findMin(node->right);
            const_cast<Key&>(node->key) = temp->key;
            node->value = temp->value;
            node->right = eraseHelper(node->right, temp->key, removed);
        }
        return node;
    }

    void inorderHelper(Node* node) const {
        if (node != nullptr) {
            inorderHelper(node->left);
            std::cout << "[" << node->key << " -> " << node->value << "] ";
            inorderHelper(node->right);
        }
    }

public:
    BinarySearchTree() : root(nullptr), total_nodes(0) {}

    ~BinarySearchTree() {
        destroyTree(root);
    }

    BinarySearchTree(const BinarySearchTree& other) {
        root = copyTree(other.root);
        total_nodes = other.total_nodes;
    }

    BinarySearchTree& operator=(const BinarySearchTree& other) {
        if (this != &other) {
            destroyTree(root);
            root = copyTree(other.root);
            total_nodes = other.total_nodes;
        }
        return this;
    }

    size_t size() const { return total_nodes; }
    bool empty() const { return total_nodes == 0; }

    void insert(const Key& key, const Value& value) {
        bool inserted = false;
        root = insertHelper(root, key, value, inserted);
        if (inserted) total_nodes++;
    }

    Value& operator[](const Key& key) {
        Node* current = root;
        while (current != nullptr) {
            if (key < current->key) current = current->left;
            else if (key > current->key) current = current->right;
            else return current->value;
        }
        insert(key, Value());
        return at(key);
    }

    Value& at(const Key& key) {
        Node* current = root;
        while (current != nullptr) {
            if (key < current->key) current = current->left;
            else if (key > current->key) current = current->right;
            else return current->value;
        }
        throw std::out_of_range("BST Error: Key out of bounds.");
    }

    bool erase(const Key& key) {
        bool removed = false;
        root = eraseHelper(root, key, removed);
        if (removed) total_nodes--;
        return removed;
    }

    void clear() {
        destroyTree(root);
        root = nullptr;
        total_nodes = 0;
    }

    void printInorder() const {
        inorderHelper(root);
        std::cout << "\n";
    }
};