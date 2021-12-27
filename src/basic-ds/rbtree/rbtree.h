#ifndef RBTREE_H
#define RBTREE_H

#include <iostream>
#include <string>

enum COLOUR {
    RED,
    BLACK
};

class RBTreeNode {
    private:
        RBTreeNode *left;
        RBTreeNode *right;
        RBTreeNode *parent;
        int data;
        // std::string colour;
        int colour;

    public:
        friend class RBTree;

        RBTreeNode(int data) : left(nullptr), right(nullptr), parent(nullptr), colour(RED) {

        }

        ~RBTreeNode() {
            delete left;
            delete right;
            delete parent;
        }
};

class RBTree {
    private:    
        RBTreeNode *root{nullptr};
    
    public:
        RBTree() { };

        ~RBTree() {
            delete root;
        }

        void insert(int data);

        RBTreeNode *insertHelper(RBTreeNode *root, int data);
        
        RBTreeNode *insertHelper(RBTreeNode *root, RBTreeNode *node);

        RBTreeNode *rrCase(RBTreeNode *root);

        // RBTreeNode *balanceInsertRBTree(RBTreeNode *node);

        void balanceInsertRBTree(RBTreeNode *node);

        int getColor(RBTreeNode *node); 

        void setColor(RBTreeNode *node, int colour);

        void rotateLeft(RBTreeNode *node);

        void rotateRight(RBTreeNode *right);

        void inorder();

        void inorderRBTree(RBTreeNode *root);
};

RBTreeNode *RBTree::insertHelper(RBTreeNode *root, RBTreeNode *node) {
    if (root == nullptr) 
        return node;

    if (root->data > node->data) {
        root->left = insertHelper(root->left, node);
        root->left->parent = root;
    }

    else if (root->data < node->data) {
        root->right = insertHelper(root->right, node);
        root->right->parent = root;
    }
    
    return root;
}

int RBTree::getColor(RBTreeNode *node) {
    if (node == nullptr)
        return BLACK;
    
    return node->colour;
}

void RBTree::setColor(RBTreeNode *node, int colour) {
    node->colour = colour;
}

void RBTree::rotateLeft(RBTreeNode *node) {
    RBTreeNode *right_child = node->right;
    node->right = right_child->left;

    if (node->right == nullptr)
        node->right->parent = node;

    right_child->parent = node->parent;

    if (right_child->parent == nullptr) {
        root = right_child;
    } else if (right_child->parent = root->parent->left) {
        root->parent->left = right_child;
    } else {
        root->parent->right = right_child;
    }

    right_child->left = node;
    node->parent = right_child;
}

void RBTree::rotateRight(RBTreeNode *node) {
    RBTreeNode *left_child = node->left;
    node->left = left_child->right;

    if (node->left != nullptr)
        node->left->parent = node;

    left_child->parent = node->parent;

    if (left_child->parent == nullptr) {
        root = left_child;
    } else if (node = node->parent->left) {
        node->parent->left = left_child;
    } else {
        node->parent->right = left_child;
    }

    left_child->right = node;
    node->parent = left_child;
}

void RBTree::balanceInsertRBTree(RBTreeNode *node) {
    RBTreeNode *parent{nullptr};
    RBTreeNode *grandparent{nullptr};
    RBTreeNode *uncle{nullptr};

    while (node != nullptr && getColor(node) == RED && getColor(node->parent) == RED) {
        parent = node->parent;
        grandparent = parent->parent;
        if (parent = grandparent->left) {
            uncle = grandparent->right;
            if (getColor(uncle) == RED) {
                setColor(parent, BLACK);
                setColor(uncle, BLACK);
                setColor(grandparent, RED);
                node = grandparent;
            }
            else {
                /*RR case*/
                if (parent->right == node) {
                    rotateLeft(parent);
                    node = parent;
                    parent = node->parent;
                }
                /*just LL case*/
                rotateRight(grandparent);
                std::swap(parent->colour, grandparent->colour);
                node = parent;
            }
        }
        else {
            uncle = grandparent->left;
            if (getColor(uncle) == RED) {
                setColor(parent, BLACK);
                setColor(uncle, BLACK);
                setColor(grandparent, RED);
                node = grandparent;
            }
            else {
                /*RL case*/
                if (parent->left == node) {
                    rotateRight(parent);
                    node = parent;
                    parent = node->parent;
                }
                /* just RR case*/ 
                rotateLeft(grandparent);
                std::swap(parent->colour, grandparent->colour);
                node = parent;
            }
        }
    }
}

void RBTree::insert(int data) {
    if (root == nullptr) {
        this->root = new RBTreeNode(data);
        setColor(root, BLACK);
        // return root;
    }
    else {
        RBTreeNode *node = new RBTreeNode(data);
        this->root = insertHelper(this->root, node);
        balanceInsertRBTree(node);
        // return root;
    }
}

void RBTree::inorder() {
    inorderRBTree(this->root);
}

void RBTree::inorderRBTree(RBTreeNode *root) {
    if (root == nullptr)
        return;

    inorderRBTree(root->left);
    std::cout << root->data << " " << root->colour << std::endl;
    inorderRBTree(root->right);
}

#endif