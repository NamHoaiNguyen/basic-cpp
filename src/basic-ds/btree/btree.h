#ifndef BASIC_DS_BTREE_H
#define BASIC_DS_BTREE_H

#include "btree_node.h"

#include <memory>
#include <type_traits>

template<typename T>
using IsInteger = std::enable_if_t<std::is_same_v<int, T> || std::is_same_v<long, T>
                                    || std::is_same_v<long int, T> || std::is_same_v<long long, T>
                                    || std::is_same_v<float, T> || std::is_same_v<double, T>>;

template<typename U>
using AcceptType = std::enable_if_t<std::is_same_v<int, U> || std::is_same_v<long, U>
                                    || std::is_same_v<long int, U> || std::is_same_v<long long, U>>;


template<typename T, typename U, typename V>
class BTree : public std::enable_shared_from_this<BTree<T, U, V>> {
private:
    std::shared_ptr<BtreeNode<T, U, V>> root_;
    U size_;

public:
    BTree() = delete;

    template<typename T_ = T, typename = IsInteger<T_>>
    explicit BTree(U&& size) : root_(nullptr), size_(std::forward<U>(size))
    {
        
    }

    void traverse();

    // decltype(auto) search(T const& key); 

    decltype(auto) insert_node(T&& value);

    decltype(auto) split_child(T&& value);

    // decltype(auto) insert_node_not_null(T&& value);
};

// template<typename T, typename U, typename V>
// decltype(auto) BTree<T, U, V>::search(T const& key)
// {
//     auto i{0};
    
//     while (i < 2 * root_->node_size_ - 1 && key < root_->node_array_[i])
//         i++;
    
//     if (root_->node_array_[i] == key)
//         return root_->node_array_[i];

//     if (root_->leaf_ == true)
//         return nullptr;

//     return root_->node_array_[i]->search(key);
// }

template<typename T, typename U, typename V>
decltype(auto) BTree<T, U, V>::insert_node(T&& key)
{
    /*If root is null, create node*/
    if (root_ == nullptr) {
        root_ = std::make_shared<BtreeNode<T, U, V>>(std::forward<U>(size_), true);
        root_->node_array_.get()[0] = key;
        root_->current_numbers_key_in_node_ = 1;
    } 
    else {
        /*if node is full*/
        if (root_->current_numbers_key_in_node_ == 2 * root_->node_size_ - 1) {
            auto btreenode = std::make_shared<BtreeNode<T, U, V>>(std::forward<U>(size_), false);
            btreenode->child_node_[0] = root_;
            btreenode->split_child(0, root_);
        
            /*New root has two child. Decide which child have new key*/
            auto i{0};
            if (btreenode->node_array_.get()[0] < key)    i++;
            btreenode->child_node_[i]->insert_node_not_null(std::forward<T>(key));
            root_ = btreenode;
        } 
        else {
            root_->insert_node_not_null(std::forward<T>(key));
        }
    }
}

// template<typename T, typename U>
// decltype(auto) BTree<T, U>::insert_node_not_null(T&& key)
// {
//     if (root_->is_leaf == true) {
//         auto i{root_->node_size_ - 1};
//         while (i > 0 && root_->node_array_[i] > key) {
//             root_->node_array_[i + 1] = root_->node_array_[i];
//             i--;
//         } 
//         root_->node_array_[i + 1] = key;
//         // root
//     }
// }

template<typename T, typename U, typename V>
void BTree<T, U, V>::traverse()
{
    if (root_ != NULL)   
        root_->traverse();
}

#endif


