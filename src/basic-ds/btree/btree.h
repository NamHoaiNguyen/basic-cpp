#ifndef BASIC-DS_BTREE_H
#define BASIC-DS_BTREE_H

#include "btree_node.h"

#include <memory>
#include <type_traits>

template<typename T>
using IsInteger = std::enable_if_t<std::is_same_v<int, T> || std::is_same_v<long, T>>;

template<typename T>
class BTree : public std::enable_shared_from_this<T> {
private:
    std::shared_ptr<BTreeNode<T>> root;

public:
    BTree() = delete;

    template<typename T_ = T, typename IsInteger<T_>>
    explicit BTree(T&& size) : root(nullptr)
    {

    }

    void traverse();

    decltype(auto) search(T& const key); 
};

template<typename T>
decltype(auto) BTree<T>::search(T& const key)
{
    int i = 0;
    
    while (i < node_size_ && key < node_array_[i])
        i++;
    
    if (node_array_[i] == key)
        return std::shared_from_this();

    if (leaf_ == true)
        return nullptr;

    return node_array_[i]->search(key);
}

template<typename T>
void BTree<T>::traverse()
{
    return;
}

#endif