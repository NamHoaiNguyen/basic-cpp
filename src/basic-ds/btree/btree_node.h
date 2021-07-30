#ifndef BASIC_DS_BTREENODE_H
#define BASIC_DS_BTREENODE_H


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

template<typename T, typename U>
class BtreeNode 
{
private:        
    U node_size_;
    bool is_leaf_;
    unsigned int current_numbers_key_in_node_;
    std::shared_ptr<T> node_array_{nullptr};
    // std::shared_ptr<T> child_node_{nullptr};
    std::vector<std::shared_ptr<T>> child_node_;

public:
    BtreeNode() = delete;

    template<typename T_ = T, typename U_ = U,
             typename = IsInteger<T_>, typename = AcceptType<U_>>
    explicit BtreeNode(U_&& size, bool&& is_leaf) : node_size_(std::forward<T>(size)), 
                                                    is_leaf_(std::move(is_leaf)),
                                                    current_numbers_key_in_node_(0), 
                                                    node_array_(new T[2 * node_size_ - 1]),
                                                    child_node_(2 * node_size_)
    {

    }

friend class BTree;
};

#endif