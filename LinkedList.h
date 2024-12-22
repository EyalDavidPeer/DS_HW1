#pragma once

#include <iostream>
#include <memory>
#include "AVLTree.h"

// Template class representing a Sorted Linked List implemented using AVL Tree
template<typename T>
class SortedLinkedList {
    AVLTree<T> tree;

public:
    // Iterator for the Sorted Linked List
    class Iterator {
    private:
        shared_ptr<AVLNode<T>> current;

        // Helper function to find the leftmost node in the subtree
        shared_ptr<AVLNode<T>> findLeftmostNode(shared_ptr<AVLNode<T>> node) {
            while (node && node->left) {
                node = node->left;
            }
            return node;
        }

        // Helper function to find the inorder successor of a node
        shared_ptr<AVLNode<T>> inorderSuccessor(shared_ptr<AVLNode<T>> node) {
            if (!node) {
                return nullptr;
            }

            if (node->right) {
                node = node->right;
                while (node->left) {
                    node = node->left;
                }
                return node;
            }

            // Traverse up using parent pointers
            auto parent = node->parent.lock();
            while (parent && node == parent->right) {
                node = parent;
                parent = parent->parent.lock();
            }

            return parent;
        }

    public:
        explicit Iterator(shared_ptr<AVLNode<T>> start)
                : current(findLeftmostNode(start)) {}

        std::shared_ptr<typename T::element_type> operator*() {
            if (!current) {
                return nullptr;
            }

            auto shared = current->data.lock(); // Lock the weak pointer
            if (!shared) {
                throw std::runtime_error("Dereferenced expired weak_ptr");
            }
            return shared; // Return locked shared_ptr
        }

        Iterator &operator++() {
            if (current) {
                current = inorderSuccessor(current);
            }
            return *this;
        }

        bool operator!=(const Iterator &other) const {
            return current != other.current;
        }
    };

    // Insert element in sorted order
    void insert(T value, int index) {
        tree.insert(value, index);
    }

    // Remove node by index
    void remove(int index) {
        tree.remove(index);
    }

    // Search for a node by index
    bool search(int index) {
        return tree.search(index);
    }

    // Retrieve the value associated with the index
    T getValue(int index) {
        return tree.getValue(index);
    }

    // In-order traversal of the AVL tree
    void printInOrder() {
        tree.printInorder();
    }

    // Clear the tree
    void clear() {
        tree = AVLTree<T>();
    }

    Iterator begin() {
        return Iterator(tree.getRoot());
    }

    Iterator end() {
        return Iterator(nullptr);
    }
};