#pragma once

#include <iostream>
#include <memory>

using namespace std;

template<typename T>
class AVLNode {
public:
    int key;
    T data;
    shared_ptr<AVLNode<T>> left;
    shared_ptr<AVLNode<T>> right;
    int height;
    weak_ptr<AVLNode<T>> parent;

    AVLNode(T k, int index)
            : key(index), data(k), left(nullptr), right(nullptr), height(1) {
    }
};

template<typename T>
class AVLTree {
private:
    shared_ptr<AVLNode<T>> m_root;

    int height(const shared_ptr<AVLNode<T>> &node) {
        if (!node) {
            return 0;
        }
        return node->height;
    }

    shared_ptr<AVLNode<T>>
    insert(shared_ptr<AVLNode<T>> node, T data, int index) {
        if (!node) {
            return make_shared<AVLNode<T>>(data, index);
        }

        if (index < node->key) {
            node->left = insert(node->left, data, index);
            node->left->parent = weak_ptr<AVLNode<T>>(node);
        } else if (index > node->key) {
            node->right = insert(node->right, data, index);
            node->right->parent = weak_ptr<AVLNode<T>>(node);
        } else {
            node->data = data;
            return node;
        }

        node->height = 1 + max(height(node->left), height(node->right));

        int balance = balanceFactor(node);

        if (balance > 1 && index < node->left->key) {
            return rightRotate(node);
        }

        if (balance < -1 && index > node->right->key) {
            return leftRotate(node);
        }

        if (balance > 1 && index > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        if (balance < -1 && index < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    shared_ptr<AVLNode<T>> rightRotate(shared_ptr<AVLNode<T>> y) {
        shared_ptr<AVLNode<T>> x = y->left;
        shared_ptr<AVLNode<T>> T2 = x->right;

        x->right = y;
        y->left = T2;

        if (T2) {
            T2->parent = weak_ptr<AVLNode<T>>(y);
        }

        x->parent = y->parent;
        y->parent = weak_ptr<AVLNode<T>>(x);

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    shared_ptr<AVLNode<T>> leftRotate(shared_ptr<AVLNode<T>> x) {
        shared_ptr<AVLNode<T>> y = x->right;
        shared_ptr<AVLNode<T>> T2 = y->left;

        y->left = x;
        x->right = T2;

        if (T2) {
            T2->parent = weak_ptr<AVLNode<T>>(x);
        }

        y->parent = x->parent;
        x->parent = weak_ptr<AVLNode<T>>(y);

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    shared_ptr<AVLNode<T>> minValueNode(shared_ptr<AVLNode<T>> node) {
        shared_ptr<AVLNode<T>> current = node;
        while (current->left) {
            current = current->left;
        }
        return current;
    }

    shared_ptr<AVLNode<T>> deleteNode(shared_ptr<AVLNode<T>> root, int index) {
        if (!root) {
            return nullptr;
        }

        if (index < root->key) {
            root->left = deleteNode(root->left, index);
            if (root->left) {
                root->left->parent = weak_ptr<AVLNode<T>>(root);
            }
        } else if (index > root->key) {
            root->right = deleteNode(root->right, index);
            if (root->right) {
                root->right->parent = weak_ptr<AVLNode<T>>(root);
            }
        } else {
            if (!root->left || !root->right) {
                shared_ptr<AVLNode<T>> temp = root->left ? root->left
                                                         : root->right;
                if (temp) {
                    temp->parent = root->parent;
                }

                if (auto parent = root->parent.lock()) {
                    if (parent->left == root) {
                        parent->left = temp;
                    } else {
                        parent->right = temp;
                    }
                }

                return temp;
            } else {
                shared_ptr<AVLNode<T>> temp = minValueNode(root->right);
                root->key = temp->key;
                root->data = temp->data;
                root->right = deleteNode(root->right, temp->key);
                if (root->right) {
                    root->right->parent = weak_ptr<AVLNode<T>>(root);
                }
            }
        }

        root->height = 1 + max(height(root->left), height(root->right));

        int balance = balanceFactor(root);

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


    bool search(const shared_ptr<AVLNode<T>> &root, int index) const {
        if (!root) {
            return false;
        }
        if (root->key == index) {
            return true;
        }
        return index < root->key ? search(root->left, index)
                                 : search(root->right, index);
    }

    shared_ptr<AVLNode<T>>
    getNode(const shared_ptr<AVLNode<T>> &root, int index) {
        if (!root) {
            return nullptr;
        }
        if (root->key == index) {
            return root;
        }
        return index < root->key ? getNode(root->left, index)
                                 : getNode(root->right, index);
    }

    T getValue(const shared_ptr<AVLNode<T>> &root, int index) {
        if (!root) {
            return T();
        }
        if (root->key == index) {
            return root->data;
        }
        return index < root->key ? getValue(root->left, index)
                                 : getValue(root->right, index);
    }

public:
    AVLTree() : m_root(nullptr) {}

    void insert(T key, int index) {
        m_root = insert(m_root, key, index);
        m_root->parent.reset();
    }

    void remove(int key) {
        m_root = deleteNode(m_root, key);
        if (m_root) {
            m_root->parent.reset();
        }
    }

    bool search(int key) {
        return search(m_root, key);
    }

    T getValue(int index) {
        return getValue(m_root, index);
    }

    shared_ptr<AVLNode<T>> getNode(int index) {
        return getNode(m_root, index);
    }

    shared_ptr<AVLNode<T>> getRoot() const {
        return m_root;
    }

    int balanceFactor(const shared_ptr<AVLNode<T>> &node) {
        if (!node) {
            return 0;
        }
        return height(node->left) - height(node->right);
    }
};