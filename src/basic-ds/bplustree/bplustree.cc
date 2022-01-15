#include "bplustree.h"

BPlusTreeNode *BPlusTree::create_new_node() {
    BPlusTreeNode *node = new BPlusTreeNode();

    node->prev = nullptr;
    node->next = nullptr;
    node->data = new int[this->depth];
    (node->child) = new BPlusTreeNode*[this->depth + 1];

    for (int i = 0; i < this->depth; i++) {
        node->data[i] = MIN;
    }

    node->number_used_nodes = 0;

    for (int i = 0; i < this->depth + 1; i++) {
        node->child[i] = nullptr;
    }

    return node;
}

bool BPlusTree::check_node_is_full(BPlusTreeNode *node) {
    return node->number_used_nodes == this->depth ? true : false; 
}

int BPlusTree::get_number_of_key_in_node(BPlusTreeNode *node) {
    return node->number_used_nodes;
}

void BPlusTree::set_number_of_key_in_node(BPlusTreeNode *node, int value_change) {
    node->number_used_nodes = value_change;
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

    number_of_current_node_child = get_number_of_key_in_node(child);
    assert(number_of_current_node_child == this->depth);

    number_of_node_child = this->depth / 2;
    number_of_node_new_child = number_of_current_node_child - number_of_node_child;

    for (int i = number_of_node_child, j = 0; j < number_of_node_new_child; i++, j++) {
        new_child->data[j] = child->data[i];
        child->data[i] = MIN;
    } 

    set_number_of_key_in_node(child, number_of_node_child);
    set_number_of_key_in_node(new_child, number_of_node_new_child);
    new_child->isleaf = is_leaf;

    if (child->isleaf && child == this->root) {
        BPlusTreeNode *new_root = create_new_node();

        if (new_root == nullptr) {
            return;
        }

        new_root->data[0] = new_child->data[0];
        new_root->child[0] = child;
        new_root->child[1] = new_child;
        set_number_of_key_in_node(new_root, 1);
        new_root->isleaf = false;
        this->root = new_root;

        return;
    }

    if (child->isleaf) {
        new_child->next = child->next;
        if (child->next != nullptr) {
            child->next->prev = new_child;
        }
        new_child->prev = child;
        child->next = new_child;

        BPlusTreeNode *parent = find_parent(this->root, child);
        if (parent != nullptr) {
            int pos = get_number_of_key_in_node(parent);

            while (pos > 0 && parent->data[pos - 1] > new_child->data[0]) {
                parent->data[pos] = parent->data[pos - 1];
                pos--;
            }
            parent->data[pos] = new_child->data[0];

            for (int i = get_number_of_key_in_node(parent); i > pos; i--) {
                parent->child[i + 1] = parent->child[i];
            }
            parent->child[pos + 1] = new_child;
            set_number_of_key_in_node(parent, parent->number_used_nodes + 1);

            /*copy node at index 0 of new child to parent node*/
            if (check_node_is_full(parent)) {
                split_node(parent, false, new_child->data[0]);
            }
        }
    }

    /*internal node*/
    else {
        if (child == this->root) {
            BPlusTreeNode *new_root = create_new_node();

            if (new_root == nullptr) {
                return;
            }

            new_root->data[0] = new_child->data[0];
            new_root->child[0] = child;
            new_root->child[1] = new_child;
            set_number_of_key_in_node(new_root, 1);
            new_root->isleaf = false;
            this->root = new_root;

            int pos = 0;

            for (int i = 0; i < new_child->number_used_nodes - 1; i++) {
                new_child->data[i] = new_child->data[i + 1];
            }
            new_child->data[new_child->number_used_nodes - 1] = MIN;
            set_number_of_key_in_node(new_child, new_child->number_used_nodes - 1);

            for (int i = 0; i < this->depth - this->depth / 2; i++) {
                new_child->child[i] = child->child[i + (this->depth / 2 + 1)];
                child->child[i + (this->depth / 2 + 1)] = nullptr;
            }
            return ;
        }    
        
        /*If internal node,*/
        else {
            BPlusTreeNode *parent = find_parent(this->root, child);
            if (parent != nullptr) {
                for (int i = 0; i < this->depth - this->depth / 2; i++) {
                    new_child->child[i] = child->child[i + (this->depth / 2 + 1)];
                    child->child[i + (this->depth / 2 + 1)] = nullptr;
                }
                
                int pos = get_number_of_key_in_node(parent);
                while (pos > 0 && parent->data[pos - 1] > new_child->data[0]) {
                    parent->data[pos] = parent->data[pos - 1];
                    pos--;
                }
                parent->data[pos] = new_child->data[0];

                for (int i = parent->number_used_nodes; i > pos; i--) {
                    parent->child[i + 1] = parent->child[i];
                }
                parent->child[pos + 1] = new_child;
                set_number_of_key_in_node(parent, parent->number_used_nodes + 1);

                if (check_node_is_full(parent)) {
                    split_node(parent, false, new_child->data[0]);
                }

                /*move node at index 0 of new child to parent node*/
                for (int i = 0; i < new_child->number_used_nodes - 1; i++) {
                    new_child->data[i] = new_child->data[i + 1];
                }
                new_child->data[new_child->number_used_nodes - 1] = MIN;
                set_number_of_key_in_node(new_child, new_child->number_used_nodes - 1);
            }
        }
    }
}

void BPlusTree::insert_node_to_leaf(BPlusTreeNode *parent, BPlusTreeNode *child, int data) {    
    if (child->isleaf == true || child->child[0] == nullptr) {
        int pos = get_number_of_key_in_node(child);

        if (check_node_is_full(child)) {
            split_node(child, child->isleaf, data);
        }
        else {
            while (pos > 0 && child->data[pos - 1] > data) { 
                child->data[pos] = child->data[pos - 1];
                pos--;
            }
            child->data[pos] = data;
            set_number_of_key_in_node(child, child->number_used_nodes + 1);

            if (check_node_is_full(child)) {
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

void BPlusTree::insert(int data) {
    int i = 0;
    
    if (root == nullptr) {
        root = create_new_node();
        root->data[0] = data; 
        set_number_of_key_in_node(root, root->number_used_nodes + 1);
    } else {
        insert_node_to_leaf(root, root, data);
    }
}

bool BPlusTree::check_low_threadshold_in_root_node(BPlusTreeNode *node) {
    return node->number_used_nodes > this->depth / 2 - 2 ? true : false;
}

bool BPlusTree::check_low_threadshold_in_not_root_node(BPlusTreeNode *node) {
    return node->number_used_nodes > this->depth / 2 - 1 ? true : false;
}

bool BPlusTree::check_low_threadshold_in_sibling_for_borrowing(BPlusTreeNode *node) {
    return node->number_used_nodes >= this->depth / 2 ? true : false;
}

int BPlusTree::get_index_of_key_in_node(BPlusTreeNode *node, int data) {
    int left = 0;
    int right = get_number_of_key_in_node(node) - 1;
    int mid = (left + right) / 2;

    while (left <= right) {
        if (node->data[mid] == data) {
            return mid;
        }
        else if (node->data[mid] < data) {
            left = mid + 1;
        }       
        else {
            right = mid - 1;
        }
        mid = (left + right) / 2;
    }

    return -1;
}

void BPlusTree::get_key_from_left_sibling(BPlusTreeNode *child, int pos, int data) {

}

void BPlusTree::get_key_from_right_sibling(BPlusTreeNode *child, int pos, int data) {
    BPlusTreeNode *sibling = child->next;
    BPlusTreeNode *parent_child = find_parent(this->root, child);
    BPlusTreeNode *parent_sibling = find_parent(this->root, child->next);
    int pos_for_parent{0};

    if (parent_child != nullptr && parent_child == parent_sibling) {
        child->data[get_number_of_key_in_node(child)] = sibling->data[0];
        for (int i = pos; i < get_number_of_key_in_node(child); i++) {
            child->data[i] = child->data[i + 1];
        }
        child->data[get_number_of_key_in_node(child)] = MIN;

        pos_for_parent = get_number_of_key_in_node(parent_child);

        if (get_index_of_key_in_node(parent_child, data) == -1) {
            int median_key = sibling->data[0];  /*or get_number_of_key_in_node(sibling) / 2*/                        
            parent_child->data[0] = median_key;

            /*delete node in sibling*/
            for (int i = 0; i < get_number_of_key_in_node(sibling) - 1; i++) {
                sibling->data[i] = sibling->data[i + 1];
            }
            sibling->data[get_number_of_key_in_node(sibling) - 1] = MIN;
            set_number_of_key_in_node(sibling, -1);
        }
        /*Need to delete in parent*/
        else {  
            if (check_low_threadshold_in_sibling_for_borrowing(child->prev)) {
                get_key_from_left_sibling(parent_child, pos_for_parent, data);
            }
            else if (check_low_threadshold_in_sibling_for_borrowing(child->prev)) {
                get_key_from_right_sibling(parent_child, pos_for_parent, data);
            }
        }
    }
}


void BPlusTree::delete_in_leaf_node(BPlusTreeNode *child, int data) {
    bool left_sibling_lower_than_threshold{false};
    bool right_sibling_lower_than_threshold{false};

    if (child->isleaf && child == this->root) {
        int pos = get_index_of_key_in_node(child, data);

        if (pos != -1) {
            for (int i = pos; i < get_number_of_key_in_node(child) - 2; i++) {
                child->data[i] = child->data[i + 1];
            }
            child->data[get_number_of_key_in_node(child) - 1] = MIN;
        }
        else {
            std::cout << "Not having this key" << std::endl;
            return;
        }
    }
    
    /*Node is leaf*/
    if (child->isleaf) {
        /*If number of node > depth / 2*/
        if (check_low_threadshold_in_not_root_node(child)) {
            BPlusTreeNode *parent = find_parent(this->root, child);
            if (parent != nullptr) {
                /*if key not exits in parent*/
                if (get_index_of_key_in_node(parent, data) == -1) {
                    int pos = get_index_of_key_in_node(child, data);
                    if (pos != -1) {
                        child->data[pos] = MIN;
                        if (get_index_of_key_in_node(parent, data) != -1) {
                            delete_in_leaf_node(parent, data);
                        }
                    }
                    else {
                        std::cout << "Not having this key at leaf" << std::endl;
                    }
                }
                else {
                    
                }
            }
        }
        else {
            /*Find in next leaf node*/
            int pos{-1};
            int pos_for_new_node{0};
            int pos_for_parent{0};

            if ((pos = get_index_of_key_in_node(child, data)) == -1) {
                std::cout << "Not having this key in leaf" << std::endl;
                return;
            }

            if (child->prev != nullptr){
                if (check_low_threadshold_in_sibling_for_borrowing(child->prev)) {
                    BPlusTreeNode *sibling = child->prev;
                    BPlusTreeNode *parent_child = find_parent(this->root, child);
                    BPlusTreeNode *parent_sibling = find_parent(this->root, child->next);

                    if (parent_child != nullptr && parent_child == parent_sibling) {
                        // for (int i = pos; i < get_number_of_key_in_node(child) - 1; i++) {
                        //     child->data[i] = child->data[i + 1];
                        // }
                        // child->data[get_number_of_key_in_node(child) - 1] = MIN;
                        // set_number_of_key_in_node(child, -1);

                        // pos_for_new_node = get_number_of_key_in_node(child);
                        // while (pos_for_new_node > 0 && child->data[pos_for_new_node - 1] > sibling->data[0]) {
                        //     child->data[pos_for_new_node] = child->data[pos_for_new_node - 1];
                        //     pos--;
                        // }
                        // child->data[pos] = sibling->data[0];
                        // set_number_of_key_in_node(child, 1);

                        /*wrong*/
                        child->data[get_number_of_key_in_node(child)] = sibling->data[0];
                        for (int i = pos; i < get_number_of_key_in_node(child); i++) {
                            child->data[i] = child->data[i + 1];
                        }
                        child->data[get_number_of_key_in_node(child)] = MIN;

                        pos_for_parent = get_number_of_key_in_node(parent_child);

                        /*???*/
                        if ((pos = get_index_of_key_in_node(parent_child, data)) == -1) {
                            int median_key = sibling->data[get_number_of_key_in_node(sibling) / 2];                        
                            parent_child->data[0] = median_key;
                        }
                        else {
                            while (pos > 0 && child->data[pos - 1] > sibling->data[0]) {
                                child->data[pos_for_parent] = child->data[pos_for_parent - 1];
                                pos--;
                            }
                        }
                    }
                }
                else {
                    left_sibling_lower_than_threshold = true;
                }
            }
            else {
                if (check_low_threadshold_in_sibling_for_borrowing(child->next)) {
                    // BPlusTreeNode *sibling = child->next;
                    // BPlusTreeNode *parent_child = find_parent(this->root, child);
                    // BPlusTreeNode *parent_sibling = find_parent(this->root, child->next);

                    // if (parent_child != nullptr && parent_child == parent_sibling) {
                    //     child->data[get_number_of_key_in_node(child)] = sibling->data[0];
                    //     for (int i = pos; i < get_number_of_key_in_node(child); i++) {
                    //         child->data[i] = child->data[i + 1];
                    //     }
                    //     child->data[get_number_of_key_in_node(child)] = MIN;

                    //     pos_for_parent = get_number_of_key_in_node(parent_child);

                    //     if (get_index_of_key_in_node(parent_child, data) == -1) {
                    //         int median_key = sibling->data[0];  /*or get_number_of_key_in_node(sibling) / 2*/                        
                    //         parent_child->data[0] = median_key;

                    //         /*delete node in sibling*/
                    //         for (int i = 0; i < get_number_of_key_in_node(sibling) - 1; i++) {
                    //             sibling->data[i] = sibling->data[i + 1];
                    //         }
                    //         sibling->data[get_number_of_key_in_node(sibling) - 1] = MIN;
                    //         set_number_of_key_in_node(sibling, -1);
                    //     }
                    //     /*Need to delete in parent*/
                    //     else {  
                            
                    //     }
                    // }
                    get_key_from_right_sibling(child, pos, data);
                }
                else {
                    right_sibling_lower_than_threshold = true;
                }
            }
            // if (check_low_threadshold_in_sibling_for_borrowing(child->prev)) {
            //     BPlusTreeNode *sibling = child->prev;
            //     BPlusTreeNode *parent_child = find_parent(this->root, child);
            //     BPlusTreeNode *parent_sibling = find_parent(this->root, child->next);

            //     if (parent_child != nullptr && parent_child == parent_sibling) {
            //         for (int i = pos; i < get_number_of_key_in_node(child) - 2; i++) {
            //             child->data[i] = child->data[i + 1];
            //         }
            //         child->data[get_number_of_key_in_node(child) - 1] = MIN;
            //         set_number_of_key_in_node(child, -1);

            //         pos_for_new_node = get_number_of_key_in_node(child);
            //         while (pos_for_new_node > 0 && child->data[pos_for_new_node - 1] > sibling->data[0]) {
            //             child->data[pos_for_new_node] = child->data[pos_for_new_node - 1];
            //             pos--;
            //         }
            //         child->data[pos] = sibling->data[0];
            //         set_number_of_key_in_node(child, 1);

            //         if (get_index_of_key_in_node(parent_child, data) != -1) {
            //             int median_key = sibling->data[get_number_of_key_in_node(sibling) / 2];                        

            //         }
            //         else {
                        
            //         }

            //         pos_for_parent = get_number_of_key_in_node(parent_child);
            //         while (pos_for_parent > 0 && parent_child->data[pos_for_parent - 1] > median_key) {
            //             parent_child->data[pos_for_parent] = parent_child->data[pos_for_parent - 1];
            //             pos_for_parent--;
            //         }

            //         // set_number_of_key_in_node(parent, parent->number_used_nodes + 1);
            //     }
            // }
            /*Find in prev leaf node*/
            // else if (check_low_threadshold_in_sibling_for_borrowing(child->prev)) {

            // }
            /*Merge child and sibling and delete one node in parent*/
            if (left_sibling_lower_than_threshold && right_sibling_lower_than_threshold) {
                
            }
        }
    }
}

void BPlusTree::delete_node(int data) {
    if (root == nullptr) {
        return;
    }

    delete_in_leaf_node(this->root, data);
}

void BPlusTree::delete_node(int data) {
    if (root == nullptr) {
        return;
    }

    delete_in_leaf_node(this->root, data);
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