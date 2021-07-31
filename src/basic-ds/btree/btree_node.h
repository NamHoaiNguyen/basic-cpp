#ifndef BASIC_DS_BTREENODE_H
#define BASIC_DS_BTREENODE_H

#include <iostream>
#include <memory>
#include <type_traits>
#include <vector>

template<typename T>
using IsInteger = std::enable_if_t<std::is_same_v<int, T> || std::is_same_v<long, T>
                                    || std::is_same_v<long int, T> || std::is_same_v<long long, T>
                                    || std::is_same_v<float, T> || std::is_same_v<double, T>>;

template<typename U>
using AcceptType = std::enable_if_t<std::is_same_v<int, U> || std::is_same_v<long, U>
                                    || std::is_same_v<long int, U> || std::is_same_v<long long, U>>;

template<typename V>
using CheckBoolean = std::enable_if<std::is_same_v<bool, V> || std::is_same_v<bool, V>>;

template<typename T, typename U, typename V>
class BtreeNode : public std::enable_shared_from_this<BtreeNode<T, U, V>>
{ 
private:        
    U node_size_;
    bool is_leaf_;
    unsigned int current_numbers_key_in_node_;
    std::shared_ptr<T> node_array_;
    std::vector<std::shared_ptr<BtreeNode<T, U, V>>> child_node_;
    // std::shared_ptr<std::shared_ptr<BtreeNode<T, U>>> child_node_;

public:
    BtreeNode() = delete;

    template<typename T_ = T, typename U_ = U, typename V_ = V,
             typename = IsInteger<T_>, typename = AcceptType<U_>, typename = CheckBoolean<V_>>
    explicit BtreeNode(U_&& size, V_&& is_leaf) : node_size_(std::forward<U_>(size)), 
                                                    is_leaf_(std::move(is_leaf)),
                                                    current_numbers_key_in_node_(1), 
                                                    node_array_(new T[2 * node_size_- 1]),
                                                    child_node_(2 * node_size_, nullptr)
    {

    }

    void traverse();

    decltype(auto) search(T const& key); 

    decltype(auto) split_child(int i, std::shared_ptr<BtreeNode<T, U, V>> &child_node_i);

    decltype(auto) insert_node_not_null(T&& key);

template<typename, typename, typename>
friend class BTree;
};

template<typename T, typename U, typename V>
decltype(auto) BtreeNode<T, U, V>::search(T const& key)
{
    auto i{0};
    
    while (i < current_numbers_key_in_node_ && key < node_array_[i])
        i++;
    
    if (node_array_[i] == key)
        return this->shared_from_this();

    if (is_leaf_ == true)
        return nullptr;

    return node_array_[i]->search(key);
}

template<typename T, typename U, typename V>
decltype(auto) BtreeNode<T, U, V>::split_child(int i, std::shared_ptr<BtreeNode<T, U, V>> &child_node_i)
{
    auto node_child = std::make_shared<BtreeNode<T, U, V>>(std::forward<U>(child_node_i->node_size_),
                                                           std::forward<V>(child_node_i->is_leaf_));
    node_child->current_numbers_key_in_node_ = node_child->node_size_ - 1;

    for (auto j = 0; j < node_size_ - 1; j++) {
        node_child->node_array_.get()[j] = child_node_i->node_array_.get()[j + node_size_];
    }

    if (child_node_i->is_leaf_ == false) {
        for (auto j = 0; j < node_size_; j++) {
            node_child->child_node_[j] = child_node_i->child_node_[j + node_size_];
        }
    }
    child_node_i->current_numbers_key_in_node_ = node_size_ - 1;
    for (auto j = current_numbers_key_in_node_; j >= i + 1; j--) {
        child_node_[j + 1] = child_node_[j];
    }

    node_array_.get()[i] = child_node_i->node_array_.get()[i];
    current_numbers_key_in_node_ += 1;
}


template<typename T, typename U, typename V>
decltype(auto) BtreeNode<T, U, V>::insert_node_not_null(T&& key)
{
    auto i{node_size_ - 1};

    if (is_leaf_ == true) {
        while (i >= 0 && node_array_.get()[i] > key) {
            node_array_.get()[i + 1] = node_array_.get()[i];
            i--;
        } 
        node_array_.get()[i + 1] = key;
        current_numbers_key_in_node_ += 1;

        return;
    }
    else {
        while (i >= 0 && node_array_.get()[i] > key)  i--;
        if (child_node_[i + 1]->current_numbers_key_in_node_ == 2 * node_size_ - 1) {
            split_child(i + 1, child_node_[i + 1]);
            if (node_array_.get()[i + 1] < key)   i++;
        }
        else {
            child_node_[i + 1]->insert_node_not_null(std::forward<T>(key));
        }
    }
}

template<typename T, typename U, typename V>
void BtreeNode<T, U, V>::traverse()
{
    auto i{0};
    for (i = 0; i < i < current_numbers_key_in_node_; i++) {
        if (is_leaf_ == false)
            child_node_[i]->traverse();
        std::cout << " " << node_array_.get()[i];
    }

    if (is_leaf_ == false) 
        child_node_[i]->traverse();
    return;
}

#endif