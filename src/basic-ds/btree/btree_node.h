#ifndef BASIC-DS_BTREENODE_H
#define BASIC-DS_BTREENODE_H

#include <memory>
#include <type_traits>

template<typename T>
using IsInteger = std::enable_if_t<std::is_unsigned_v<T>>;

template<typename T>
class BtreeNode {
private:        
    unsigned int node_size_;
    bool is_leaf_;
    std::shared_ptr<T> node_array_{nullptr};
    std::shared_ptr<T>> child_node_{nullptr};

public:
    BtreeNode() = delete;
    

    explicit<typename T_ = T, typename IsInteger<T_>>
    BtreeNode(T_&& size) : node_size_(std::forward<T>(size)), node_array_(new T_[2 * node_size_ - 1])
                            , child_node_(new T_[2 * node_size]), is_leaf_(false)
    {

    }

friend class BTree<T>

};

#endif