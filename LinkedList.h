#pragma once

#include <iostream>
#include <memory>
#include "AVLTree.h"

template<typename T>
class SortedLinkedList {
    AVLTree<T> tree;

public:
    class Iterator {
    private:
        shared_ptr<AVLNode<T>> current;

        shared_ptr<AVLNode<T>> findLeftmostNode(shared_ptr<AVLNode<T>> node) {
            while (node && node->left) {
                node = node->left;
            }
            return node;
        }

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

            auto shared = current->data.lock();
            if (!shared) {
                throw std::runtime_error("Dereferenced expired weak_ptr");
            }
            return shared;
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

    void insert(T value, int index) {
        tree.insert(value, index);
    }

    void remove(int index) {
        tree.remove(index);
    }

    bool search(int index) {
        return tree.search(index);
    }

    T getValue(int index) {
        return tree.getValue(index);
    }

    void printInOrder() {
        tree.printInorder();
    }

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