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

        BPlusTreeNode* create_new_node();

        bool check_node_is_full(BPlusTreeNode *node);

        void insert_node_to_leaf(BPlusTreeNode *parent, BPlusTreeNode *child, int data);

        void split_node(BPlusTreeNode *child, bool is_leaf, int data);

        BPlusTreeNode *find_parent(BPlusTreeNode *root, BPlusTreeNode *node);\

        int get_number_of_key_in_node(BPlusTreeNode *node);

        void set_number_of_key_in_node(BPlusTreeNode *node, int value_change);

        void delete_node(int data);

        void delete_in_leaf_node(BPlusTreeNode *parent, int data);

        bool check_value_in_parent(BPlusTreeNode *child);

        bool check_low_threadshold_in_root_node(BPlusTreeNode *node);

        bool check_low_threadshold_in_not_root_node(BPlusTreeNode *node);

        bool check_low_threadshold_in_sibling_for_borrowing(BPlusTreeNode *node);
        
        int get_index_of_key_in_node(BPlusTreeNode *node, int data);

        bool get_key_from_left_sibling(BPlusTreeNode *node, int pos, int data);

        bool get_key_from_right_sibling(BPlusTreeNode *node, int pos, int data);

        void delete_key_in_internal_node(BPlusTreeNode* node, int data);

        int get_index_pointer_in_parent(BPlusTreeNode* parent, BPlusTreeNode *child);

        void merge_child_node_with_sibling(BPlusTreeNode* node, int pos, int data);

        void print_tree();

        void print_tree_helper(BPlusTreeNode *root);

        friend class BPlusTreeNode;
};

#endif
