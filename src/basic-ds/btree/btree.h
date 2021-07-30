#ifndef BASIC-DS_BTREE_H
#define BASIC-DS_BTREE_H

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


template<typename T, typename U>
class BTree : public std::enable_shared_from_this<T, U> {
private:
    std::shared_ptr<BTreeNode<T, U>> root_;
    U size_;

public:
    BTree() = delete;

    template<typename T_ = T, typename IsInteger<T_>>
    explicit BTree(U&& size) : root(nullptr), size_(std::forward<U>(size))
    {

    }

    void traverse();

    decltype(auto) search(T& const key); 

    decltype(auto) add_node(T&& value);

    decltype(auto) split_child(T&& value);
};

template<typename T, typename U>
decltype(auto) BTree<T, U>::search(T& const key)
{
    auto i = {0};
    
    while (i < node_size_ && key < node_array_[i])
        i++;
    
    if (node_array_[i] == key)
        return std::shared_from_this();

    if (leaf_ == true)
        return nullptr;

    return node_array_[i]->search(key);
}

template<typename T, typename U>
decltype(auto) BTree<T, U>::add_node(T&& key)
{
    if (root == nullptr) {
        root = std::make_shared<BtreeNode<T, U>>(size_, true);
    } 
    else {

    }
}

template<typename T, typename U>
void BTree<T, U>::traverse()
{
    return;
}

#endif