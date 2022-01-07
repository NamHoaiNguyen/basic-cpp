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
            free(prev);
            free(next);
            free(*child);
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

        bool check_free_space_in_node(BPlusTreeNode *node);

        void insert_node_nonfull(BPlusTreeNode *node, int data);

        void insert_node_to_leaf(BPlusTreeNode *parent, BPlusTreeNode *child, int data);

        void split_node(BPlusTreeNode *child, bool is_leaf, int data);

        BPlusTreeNode *find_parent(BPlusTreeNode *root, BPlusTreeNode *node);

        void print_tree();

        void print_tree_helper(BPlusTreeNode *root);

        friend class BPlusTreeNode;
};

#endif 

BPlusTreeNode *BPlusTree::create_new_node() {
    BPlusTreeNode *node = new BPlusTreeNode();

    node->prev = nullptr;
    node->next = nullptr;
    node->data = (int *)malloc(sizeof(int) * (this->depth));
    (node->child) = (struct BPlusTreeNode**) malloc(sizeof(BPlusTreeNode*) * (this->depth + 1));

    // node->data[0] = data;
    for (int i = 0; i < this->depth; i++) {
        node->data[i] = MIN;
    }

    node->number_used_nodes = 0;
    // node->data[0] = data;

    for (int i = 0; i < this->depth + 1; i++) {
        node->child[i] = nullptr;
    }

    return node;
}

bool BPlusTree::check_free_space_in_node(BPlusTreeNode *node) {
    return node->number_used_nodes == this->depth 
             ? true : false; 
}

void BPlusTree::insert_node_to_leaf(BPlusTreeNode *parent, BPlusTreeNode *child, int data) {    
    if (child->isleaf == true || child->child[0] == nullptr) {
        int pos = child->number_used_nodes;
        if (check_free_space_in_node(child)) {
            // split_node(parent, child, data);
            split_node(child, child->isleaf, data);
            // insert_node_to_internal(find_parent(root, parent), parent, data);
        }
        else {
            while (pos > 0 && child->data[pos - 1] > data) { 
                child->data[pos] = child->data[pos - 1];
                pos--;
            }
            child->data[pos] = data;
            child->number_used_nodes++;
            if (check_free_space_in_node(child)) {
                split_node(child, child->isleaf, data);
            }
        }
    }
    else {
        int pos = 0;

        while (pos < child->number_used_nodes && child->data[pos] < data) {
            pos++;
        }

        if (child->child[pos] != nullptr) {
            parent = child;
            child = child->child[pos];
        }
        insert_node_to_leaf(parent, child, data);
    }
}

BPlusTreeNode *BPlusTree::find_parent(BPlusTreeNode *parent, BPlusTreeNode *child) {
    BPlusTreeNode *is_parent;
    
    if (parent->isleaf || child == this->root) {
        return nullptr;
    }

    for (int i = 0; i < parent->number_used_nodes + 1; i++) {
        if (parent->child[i] == child) {
            is_parent = parent;
            return is_parent;
        }
        else {
            is_parent = find_parent(parent->child[i], child);
            if (is_parent != nullptr) {
                return is_parent;
            } 
        }
    }

    return is_parent;
}

void BPlusTree::split_node(BPlusTreeNode *child, bool is_leaf, int data) {
    int pos{0};
    int j{0};

    int number_of_current_node_child{0};
    int number_of_node_child{0};
    int number_of_node_new_child{0};

    BPlusTreeNode *new_child = create_new_node();

    if (new_child == nullptr) 
        return;

    number_of_current_node_child = child->number_used_nodes;
    assert(number_of_current_node_child == this->depth);

    number_of_node_child = this->depth / 2;
    number_of_node_new_child = number_of_current_node_child - number_of_node_child;

    for (int i = number_of_node_child, j = 0; j < number_of_node_new_child; i++, j++) {
        new_child->data[j] = child->data[i];
        child->data[i] = MIN;
    } 

    child->number_used_nodes = number_of_node_child;
    new_child->number_used_nodes = number_of_node_new_child;
    new_child->isleaf = is_leaf;

    if (child->isleaf && child == this->root) {
        BPlusTreeNode *new_root = create_new_node();

        if (new_root == nullptr) {
            return;
        }

        new_root->data[0] = new_child->data[0];
        new_root->child[0] = child;
        new_root->child[1] = new_child;
        new_root->number_used_nodes = 1;
        new_root->isleaf = false;
        this->root = new_root;

        return;
    }

    if (child->isleaf) {
        BPlusTreeNode *parent = find_parent(this->root, child);
        if (parent != nullptr) {
            int pos = parent->number_used_nodes;

            while (pos > 0 && parent->data[pos - 1] > new_child->data[0]) {
                parent->data[pos] = parent->data[pos - 1];
                pos--;
            }
            parent->data[pos] = new_child->data[0];

            for (int i = parent->number_used_nodes; i > pos; i--) {
                parent->child[i + 1] = parent->child[i];
            }
            parent->child[pos + 1] = new_child;
            parent->number_used_nodes += 1;

            if (check_free_space_in_node(parent)) {
                split_node(parent, parent->isleaf, new_child->data[0]);
            }
        }
    }
    else {
        for (int i = 0; i < (this->depth + 1) / 2; i++) {
            new_child->child[i] = child->child[i + ((this->depth) / 2)];
        }

        if (child == this->root) {
            std::cout << "Vao day a" << std::endl; 
            BPlusTreeNode *new_root = create_new_node();

            if (new_root == nullptr) {
                return;
            }

            new_root->data[0] = new_child->data[0];
            new_root->child[0] = child;
            new_root->child[1] = new_child;
            new_root->number_used_nodes = 1;
            new_root->isleaf = false;
            this->root = new_root;

            int pos = 0;

            for (int i = 0; i < new_child->number_used_nodes - 1; i++) {
                new_child->data[i] = new_child->data[i + 1];
            }
            new_child->data[new_child->number_used_nodes - 1] = MIN;
            new_child->number_used_nodes--;

            for (int i = 0; i < this->depth - this->depth / 2; i++) {
                new_child->child[i] = child->child[i + (this->depth / 2 + 1)];
                child->child[i + (this->depth / 2 + 1)] = nullptr;
            }
            return ;
        }    
        else {
            BPlusTreeNode *parent = find_parent(this->root, child);
            if (parent != nullptr) {
                if (check_free_space_in_node(parent)) {
                    split_node(parent, false, new_child->data[0]);
                }
                int pos = parent->number_used_nodes;
                    while (pos > 0 && parent->data[pos] > new_child->data[0]) {
                        parent->data[pos] = parent->data[pos - 1];
                        pos--;
                    }
                    parent->data[pos] = new_child->data[0];

                    for (int i = parent->number_used_nodes; i > pos; i--) {
                        parent->child[i + 1] = parent->child[i];
                    }
                    parent->child[pos + 1] = new_child;


                if (check_free_space_in_node(parent)) {
                    split_node(parent, false, new_child->data[0]);
                }
            }
        }
    }
}

void BPlusTree::insert(int data) {
    int i = 0;
    
    if (root == nullptr) {
        root = create_new_node();
        root->data[0] = data; 
        root->number_used_nodes++;
    } else {
        insert_node_to_leaf(root, root, data);
    }
}

void BPlusTree::print_tree() {
    print_tree_helper(this->root);
}

void BPlusTree::print_tree_helper(BPlusTreeNode *root) {
    if (root != nullptr) {
        for(int i = 0; i < root->number_used_nodes; i++) {
            if (root->data[i] != MIN){
                std::cout << root->data[i] << " " << std::endl;
            }
            print_tree_helper(root->child[i]);
            if (i == root->number_used_nodes - 1) {
                print_tree_helper(root->child[i + 1]);
            }
        }
    }
}