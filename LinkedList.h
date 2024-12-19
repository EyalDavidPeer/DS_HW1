#pragma once

#include <iostream>
#include "AVLTree.h"
//#include "AVLTree_New.h"

template<typename T>
class SortedLinkedList {
    AVLTree<T> tree;

public:
    class Iterator {
    private:
        AVLNode<T> *current;

        // Helper function to find the leftmost node in the subtree
        AVLNode<T> *findLeftmostNode(AVLNode<T> *node) {
            while (node && node->left != nullptr) {
                node = node->left;
            }
            return node;
        }

        // Find the in-order successor
        AVLNode<T> *inorderSuccessor(AVLNode<T> *node) {
            if (node == nullptr) { return nullptr; }

            if (node->right != nullptr) {
                node = node->right;
                while (node->left != nullptr) {
                    node = node->left;
                }
                return node;
            }

            AVLNode<T> *parent = node->parent;
            while (parent != nullptr && node == parent->right) {
                node = parent;
                parent = parent->parent;
            }
            return parent;
        }

    public:
        explicit Iterator(AVLNode<T> *start) : current(
                findLeftmostNode(start)) {}

        T &operator*() {
            return current->data;
        }

        Iterator &operator++() {
            if (current) {
                current = inorderSuccessor(current);
                if (current == nullptr) {
                    // End of the list, we can stop iteration here
                    // This could also be handled by adjusting the `end()` iterator logic.
                }
            }
            return *this;
        }

        bool operator!=(const Iterator &other) const {
            return current != other.current;
        }
    };


    // Insert element in sorted order (leveraging AVLTree tree)
    void insert(T value, int index) {
        tree.insert(value, index); // Insert the value into AVLTree tree
    }

    // Remove node by index (O(log n) removal)
    void remove(int index) {
//        if (index == 478719) {
//            printf("\n\n\nStarting the removal\n\n\n");
//        }
        tree.remove(index); // Remove the node with this index from AVLTree tree
    }

    // Search for a node by index (O(log n) getValue)
    bool search(int index) {
        return tree.search(index); // Search for the index in AVLTree tree
    }

    // Retrieve the value associated with the index
    T getValue(int index) {
        return tree.getValue(index); // Get the value from AVLTree tree by index
    }

    // In-order traversal of the AVLTree tree (to print elements in sorted order)
    void printInOrder() {
        tree.printInorder(); // Perform in-order traversal of the AVLTree tree
    }

    // Clear the tree (deletes all nodes in AVLTree tree)
    void clear() {
        tree = AVLTree<T>(); // Reinitialize tree to clear all elements
    }

    Iterator begin() {
        return Iterator(
                tree.getRoot()); // Assuming getRoot() gives the root node of the AVLTree tree
    }


    // End iterator for the end of the traversal
    Iterator end() {
        return Iterator(
                nullptr); // End is represented by a nullptr (after the last element)
    }
};