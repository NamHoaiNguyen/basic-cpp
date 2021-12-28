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

        RBTreeNode(int data) : left(nullptr), right(nullptr), parent(nullptr), data{data}, colour(RED) {

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

        // RBTreeNode *insertHelper(RBTreeNode *root, int data);
        
        RBTreeNode *insertHelper(RBTreeNode *root, RBTreeNode *node);

        RBTreeNode *rrCase(RBTreeNode *root);

        // RBTreeNode *balanceInsertRBTree(RBTreeNode *node);

        void balanceInsertRBTree(RBTreeNode *&node);

        int getColor(RBTreeNode *node); 

        void setColor(RBTreeNode *node, int colour);

        void rotateLeft(RBTreeNode *node);

        void rotateRight(RBTreeNode *right);

        void inorder();

        void inorderRBTree(RBTreeNode *root);
};


#endif