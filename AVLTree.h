#pragma once

#include <algorithm>
#include <iostream>

using namespace std;

// Template class representing a node in the AVL tree
template<typename T>
class AVLNode {
public:
    int key;
    T data; // The value of the node
    AVLNode *left; // Pointer to the left child
    AVLNode *right; // Pointer to the right child
    int height; // Height of the node in the tree
    AVLNode *parent; // Pointer to the parent node

    // Constructor to initialize a node with a given data
    AVLNode(T k, int index)
            : key(index), data(k), left(nullptr), right(nullptr), height(1),
              parent(nullptr) {
    }
};


// Template class representing the AVL tree
template<typename T>
class AVLTree {
private:
    // Pointer to the root of the tree
    AVLNode<T> *m_root;

    // Function to get the height of a node
    int height(AVLNode<T> *node) {
        if (node == nullptr) {
            return 0;
        }
        return node->height;
    }

    // Function to get the balance factor of a node
    int balanceFactor(AVLNode<T> *node) {
        if (node == nullptr) {
            return 0;
        }
        return height(node->left) - height(node->right);
    }

    // Function to perform a right rotation on a subtree
    AVLNode<T> *insert(AVLNode<T> *node, T key, int index) {
        // Perform the normal BST insertion
        if (node == nullptr) {
            return new AVLNode<T>(key, index);
        }

        if (index < node->key) {
            node->left = insert(node->left, key, index);
            node->left->parent = node;  // Set parent pointer of the left child
        } else if (index > node->key) {
            node->right = insert(node->right, key, index);
            node->right->parent = node;  // Set parent pointer of the right child
        } else {
            return node;
        }

        // Update height of this ancestor node
        node->height = 1 + max(height(node->left), height(node->right));

        // Get the balance factor of this ancestor node
        int balance = balanceFactor(node);

        // If this node becomes unbalanced, then there are 4 cases

        // Left Left Case
        if (balance > 1 && index < node->left->key) {
            return rightRotate(node);
        }

        // Right Right Case
        if (balance < -1 && index > node->right->key) {
            return leftRotate(node);
        }

        // Left Right Case
        if (balance > 1 && index > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && index < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    AVLNode<T> *rightRotate(AVLNode<T> *y) {
        AVLNode<T> *x = y->left;
        AVLNode<T> *T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update parent pointers
        if (T2) { T2->parent = y; }
        x->parent = y->parent;
        y->parent = x;

        // Update heights
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        // Return new root
        return x;
    }

    AVLNode<T> *leftRotate(AVLNode<T> *x) {
        AVLNode<T> *y = x->right;
        AVLNode<T> *T2 = y->left;

        y->left = x;
        x->right = T2;

        // Update parent pointers
        if (T2) { T2->parent = x; }
        y->parent = x->parent;
        x->parent = y;

        // Update heights
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        // Return new root
        return y;
    }

    // Function to find the node with the minimum data value
    AVLNode<T> *minValueNode(AVLNode<T> *node) {
        AVLNode<T> *current = node;
        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    // Function to delete a data from the subtree rooted with root
    AVLNode<T> *deleteNode(AVLNode<T> *root, int index) {
        if (root == nullptr) {
            return nullptr;
        }

        // Perform standard BST deletion
        if (index < root->key) {
            root->left = deleteNode(root->left, index);
        } else if (index > root->key) {
            root->right = deleteNode(root->right, index);
        } else {
            // Node with one child or no child
            if (root->left == nullptr || root->right == nullptr) {
                AVLNode<T> *temp = root->left ? root->left : root->right;
                if (temp == nullptr) { // No child case
                    temp = root;
                    root = nullptr;
                } else { // One child case
                    *root = *temp;
                }
                delete temp;
            } else {
                // Node with two children
                AVLNode<T> *temp = minValueNode(root->right);
                root->data = temp->data; // Copy the inorder successor's value
                root->right = deleteNode(root->right, temp->key);
            }
        }

        // If the tree has only one node, return it
        if (root == nullptr) {
            return root;
        }

        // Update height of the current node
        root->height = 1 + max(height(root->left), height(root->right));

        // Get balance factor to check if the node became unbalanced
        int balance = balanceFactor(root);

        // Perform rotations to balance the tree
        if (balance > 1 && balanceFactor(root->left) >= 0) {
            return rightRotate(root);
        }

        if (balance > 1 && balanceFactor(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        if (balance < -1 && balanceFactor(root->right) <= 0) {
            return leftRotate(root);
        }

        if (balance < -1 && balanceFactor(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    // Function to perform inorder traversal of the tree
    void inorder(const AVLNode<T> *root) const {
        if (root != nullptr) {
            inorder(root->left);
            cout << root->data << " ";
            inorder(root->right);
        }
    }

    // Function to search for a data in the subtree rooted with root
    bool search(const AVLNode<T> *root, int index) const {
        if (root == nullptr) {
            return false;
        }
        if (root->key == index) {
            return true;
        }

        return index < root->key ? search(root->left, index)
                                 : search(root->right, index);
    }

    T getValue(AVLNode<T> *root, int index) {
        if (root == nullptr) {
            return nullptr;
        }
        if (root->key == index) {
            return root->data;
        }
        return index < root->key ? getValue(root->left, index)
                                 : getValue(root->right, index);
    }

public:
    // Constructor to initialize the AVL tree
    AVLTree()
            : m_root(nullptr) {
    }

    // Function to insert a data into the AVL tree
    void insert(T key, int index) {
        m_root = insert(m_root, key, index);
    }

    // Function to remove a data from the AVL tree
    void remove(int key) {
        m_root = deleteNode(m_root, key);
    }

    // Function to search for a data in the AVL tree
    bool search(int key) {
        return search(m_root, key);
    }

    // Function to get the value associated with a key
    T getValue(int index) {
        return getValue(m_root, index);
    }

    // Function to print the inorder traversal of the AVL tree
    void printInorder() {
        inorder(m_root);
        cout << endl;
    }

    // Getter for root node for external access (e.g., in iterators)
    AVLNode<T> *getRoot() const {
        return m_root;
    }
};
