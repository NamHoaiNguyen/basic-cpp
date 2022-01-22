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

bool BPlusTree::get_key_from_left_sibling(BPlusTreeNode *child, int pos, int data) {
    if (child->prev == nullptr || !check_low_threadshold_in_sibling_for_borrowing(child->prev)) {
        return false;    
    }

    BPlusTreeNode *sibling = child->prev;
    BPlusTreeNode *parent_child = find_parent(this->root, child);
    BPlusTreeNode *parent_sibling = find_parent(this->root, child->prev);
    int pos_for_parent{0};

    if (parent_child != nullptr && parent_child == parent_sibling) {
        /*wrong*/
        child->data[get_number_of_key_in_node(child)] = sibling->data[0];
        for (int i = pos; i > 0; i--) {
            child->data[i] = child->data[i - 1];
        }
        child->data[0] = sibling->data[get_number_of_key_in_node(sibling) - 1];

        /*delete node in sibling*/
        int median_key = sibling->data[get_number_of_key_in_node(sibling) / 2];
        for (int i = 0; i < get_number_of_key_in_node(sibling) - 1; i++) {
            sibling->data[i] = sibling->data[i + 1];
        }
        sibling->data[get_number_of_key_in_node(sibling) - 1] = MIN;
        set_number_of_key_in_node(sibling, -1);

        pos_for_parent = get_number_of_key_in_node(parent_child);
        /*if parent doesn't have node*/
        if (get_index_of_key_in_node(parent_child, data) == -1) {
            parent_child->data[0] = median_key; /*child->data[0];*/
        }
        /*Need to delete in parent*/
        else { 
            /*why do it here?*/ 
            // if (!check_low_threadshold_in_not_root_node(parent_child)) {
            //     if (check_low_threadshold_in_sibling_for_borrowing(parent_child->prev)) {
            //         get_key_from_left_sibling(parent_child, pos_for_parent, data);
            //     }
            //     else if (check_low_threadshold_in_sibling_for_borrowing(parent_child->next)) {
            //         get_key_from_right_sibling(parent_child, pos_for_parent, data);
            //     }
            // }
            int borrow_key = sibling->data[0];
            parent_child->data[0] = borrow_key; 
        }

        delete_key_in_internal_node(parent_child, data);
    }

    return true;
}

bool BPlusTree::get_key_from_right_sibling(BPlusTreeNode *child, int pos, int data) {
    if (child->next == nullptr || !check_low_threadshold_in_sibling_for_borrowing(child->next)) {
        return false;
    }
     
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

        /*delete node in sibling*/
        int median_key = sibling->data[get_number_of_key_in_node(sibling) / 2];
        for (int i = 0; i < get_number_of_key_in_node(sibling) - 1; i++) {
            sibling->data[i] = sibling->data[i + 1];
        }
        sibling->data[get_number_of_key_in_node(sibling) - 1] = MIN;
        set_number_of_key_in_node(sibling, -1);
        
        pos_for_parent = get_number_of_key_in_node(parent_child);

        /*if parent doesn't have key
        Add the median key of the sibling node to the parent*/
        if (get_index_of_key_in_node(parent_child, data) == -1) {
            // int median_key = sibling->data[0];  /*or get_number_of_key_in_node(sibling) / 2*/                        
            parent_child->data[0] = median_key;
        }
        /*Need to delete in parent*/
        else { 
            int borrow_key = sibling->data[0];
            parent_child->data[0] = borrow_key; 
            // if (check_low_threadshold_in_not_root_node(parent_child)) {
            //     int borrow_key = sibling->data[0];
            //     parent_child->data[0] = borrow_key;
            // }
            // else {

            // }
            // if (!check_low_threadshold_in_not_root_node(parent_child)) {
            //     if (check_low_threadshold_in_sibling_for_borrowing(parent_child->prev)) {
            //         get_key_from_left_sibling(parent_child, pos_for_parent, data);
            //     }
            //     else if (check_low_threadshold_in_sibling_for_borrowing(parent_child->next)) {
            //         get_key_from_right_sibling(parent_child, pos_for_parent, data);
            //     }
            // }
        }

        delete_key_in_internal_node(parent_child, data);
    }

    return true;
}

bool BPlusTree::delete_key_in_internal_node(BPlusTreeNode* node, int data) {
    int index{-1};
    
    if ((index = get_index_of_key_in_node(node, data)) == -1) 
        return;

    /*Change the data in parent with the inorder successor*/
    node->data[index] = node->child[index + 1]->data[0];

    delete_key_in_internal_node(find_parent(this->root, node), data);
}

void BPlusTree::delete_in_leaf_node(BPlusTreeNode *child, int data) {
    bool left_sibling_higher_than_threshold{false};
    bool right_sibling_higher_than_threshold{false};

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
                /*if key exits in node*/
                int key_index_in_child{-1};
                if ((key_index_in_child = get_index_of_key_in_node(child, data)) != -1) {
                    int pos_index_in_parent = get_index_of_key_in_node(parent, data);
                    /*parent has key*/
                    // if (pos_index_in_parent != -1) {
                    //     for (int i = key_index_in_child; i < get_number_of_key_in_node(child) - 1; i++) {
                    //         child->data[i] = child->data[i + 1];
                    //     }
                    //     child->data[get_number_of_key_in_node(child) - 1] = MIN;
                    //     child->number_used_nodes -= 1;

                    //     parent->data[pos_index_in_parent] = child->data[key_index_in_child];
                    // }
                    // /*parent doesn't has key*/
                    // else {
                    //     for (int i = key_index_in_child; i < get_number_of_key_in_node(child) - 1; i++) {
                    //         child->data[i] = child->data[i + 1];
                    //     }
                    //     child->data[get_number_of_key_in_node(child) - 1] = MIN;
                    //     child->number_used_nodes -= 1;
                    // }
                    for (int i = key_index_in_child; i < get_number_of_key_in_node(child) - 1; i++) {
                        child->data[i] = child->data[i + 1];
                    }
                    child->data[get_number_of_key_in_node(child) - 1] = MIN;
                    child->number_used_nodes -= 1;

                    /*if parent has key:
                    Fill the empty space in the internal node with the inorder successor*/
                    if (pos_index_in_parent != -1) {
                        parent->data[pos_index_in_parent] = child->data[key_index_in_child];
                    }
                }
                else {
                    std::cout << "Key not exits in node" << std::endl;      
                }
            }
        }
        else {
            /*Find in next leaf node*/
            int pos{-1};

            if ((pos = get_index_of_key_in_node(child, data)) == -1) {
                std::cout << "Not having this key in leaf" << std::endl;
                return;
            }

            if (get_key_from_left_sibling(child, pos, data)) {
                left_sibling_higher_than_threshold = true;
            }
            else {
                left_sibling_higher_than_threshold = false;
            }
            
            if (!left_sibling_higher_than_threshold){
                if (get_key_from_right_sibling(child, pos, data)) {
                    right_sibling_higher_than_threshold = true;
                }
                else {
                    right_sibling_higher_than_threshold = false;
                }
            }
            /*Merge child and sibling and delete one node in parent*/
            if (!left_sibling_higher_than_threshold && !right_sibling_higher_than_threshold) {
                for (int i = pos; i < get_number_of_key_in_node(child) - 1; i++) {
                    child->data[i] = child->data[i + 1];
                }
                child->data[get_number_of_key_in_node(child) - 1] = MIN;
                child->number_used_nodes -=1;

                /*Merge node and sibling*/
                
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