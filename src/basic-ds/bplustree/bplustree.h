#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>

#define MIN -123456789

class BPlusTreeNode {
    private:
        BPlusTreeNode **child;
        BPlusTreeNode *prev;
        BPlusTreeNode *next;

        int *data;
        int number_used_nodes;
        bool isleaf;

    public:
        BPlusTreeNode() : prev(nullptr), next(nullptr), child(nullptr), isleaf(true) {

        }

        ~BPlusTreeNode() {
            delete(prev);
            delete(next);
            delete(*child);
        }

        friend class BPlusTree;
};

class BPlusTree {
    private:
        BPlusTreeNode *root;
        int depth;
    
    public:
        BPlusTree(int depth) : root(nullptr), depth(depth) {

        }

        void insert(int data);

        BPlusTreeNode* create_new_node(int data);

        BPlusTreeNode* create_new_node();

        bool check_node_is_full(BPlusTreeNode *node);

        void insert_node_nonfull(BPlusTreeNode *node, int data);

        void insert_node_to_leaf(BPlusTreeNode *parent, BPlusTreeNode *child, int data);

        void split_node(BPlusTreeNode *child, bool is_leaf, int data);

        BPlusTreeNode *find_parent(BPlusTreeNode *root, BPlusTreeNode *node);

        void print_tree();

        void print_tree_helper(BPlusTreeNode *root);

        friend class BPlusTreeNode;
};

#endif