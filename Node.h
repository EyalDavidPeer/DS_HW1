#pragma once

#include <iostream>

//TODO: Make it doubly linked list(in order to delete the last )
template<typename T>
struct Node {
    Node<T> *next;
    Node<T> *prev;
    T data;

};

template<typename T>
class LinkedList {
    Node<T> *head;

    void deleteFromBeginning();
    void deleteFromEnd();
    void deleteFromPosition(int position);
public:
    class Iterator {
    private:
        Node<T> *current;

    public:
        explicit Iterator(Node<T> *start) : current(start) {}

        // Dereference operator to access the value at the iterator
        T &operator*() {
            return current->data;  // Assuming your Node stores T as 'data'
        }

        // Increment operator for moving to the next element in in-order traversal
        Iterator &operator++() {
            if (current) {
                current = current->next;
            }
            return *this;
        }

        bool operator!=(const Iterator &other) const {
            return current != other.current;
        }
    };

    ~LinkedList() {
        while (head) {
            Node<T> *temp = head;
            head = head->next;
            delete temp;
        }
    }

    LinkedList() : head(nullptr) {}

    void insertAtBeginning(T value);
    void insertAtEnd(T value);
    void insertAtPosition(T value, int position);
    void removeNode(Node<T> *node);
    Node<T> *getLastNode();

    Iterator begin() {
        return Iterator(
                head); // Assuming getRoot() gives the root node of the AVL tree
    }

    // End iterator for the end of the traversal
    Iterator end() {
        return Iterator(
                nullptr); // End is represented by a nullptr (after the last element)
    }

};

template<typename T>
Node<T> *LinkedList<T>::getLastNode() {
    Node<T> *temp = head;
    if (!temp) { return nullptr; }
    while (temp->next) {
        temp = temp->next;
    }
    return temp;
}

template<typename T>
void LinkedList<T>::removeNode(Node<T> *node) {
    if (!node) { return; }

    if (node == head) {
        head = node->next;
        if (head) { head->prev = nullptr; }
    } else {
        if (node->prev) { node->prev->next = node->next; }
        if (node->next) { node->next->prev = node->prev; }
    }
    delete node;
}

template<typename T>
void LinkedList<T>::insertAtBeginning(T value) {
    Node<T> *newNode = new Node<T>();
    newNode->data = value;
    newNode->next = head;
    head = newNode;
}

template<typename T>
void LinkedList<T>::insertAtEnd(T value) {
    Node<T> *newNode = new Node<T>();
    newNode->data = value;
    newNode->next = nullptr;

    // If the list is empty, update the head to the new node
    if (!head) {
        head = newNode;
        return;
    }
}

template<typename T>
void LinkedList<T>::insertAtPosition(T value, int position) {
    if (position < 1) {
        std::cout << "Position should be >= 1." << std::endl;
        return;
    }

    if (position == 1) {
        insertAtBeginning(value);
        return;
    }

    Node<T> *newNode = new Node<T>();
    newNode->data = value;

    // Traverse to the node before the desired position
    Node<T> *temp = head;
    for (int i = 1; i < position - 1 && temp; ++i) {
        temp = temp->next;
    }

    // If the position is out of range, print an error message
    if (!temp) {
        std::cout << "Position out of range." << std::endl;
        delete newNode;
        return;
    }

    // Insert the new node at the desired position
    newNode->next = temp->next;
    temp->next = newNode;
}

