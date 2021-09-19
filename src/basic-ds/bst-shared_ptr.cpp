#include <iostream>
#include <memory>

template<typename T>
class Node {
    protected:
        // std::shared_ptr<Node<T>> left;
        // std::shared_ptr<Node<T>> right;
       // std::unique_ptr<Node<T>> left;
        //std::unique_ptr<Node<T>> right;
        // T value;
    public:
        std::shared_ptr<Node<T>> left;
        std::shared_ptr<Node<T>> right;
        T value;

        Node() = default;
        explicit Node(T&& value) : value(std::forward<T>(value)), left(nullptr), right(nullptr)
        {

        }
};

template<typename T>
class Bst : public Node<T>{
    private:
    //    std::shared_ptr<Node<T>> root;
        std::shared_ptr<Node<T>> root;
    public:
        // std::unique_ptr<Node<T>> root;

        Bst();
        decltype(auto) search(T&& value);
        decltype(auto) search_node(std::shared_ptr<Node<T>> &root, T&& value);
        decltype(auto) insert(T&& value);
        decltype(auto) insert_node(std::shared_ptr<Node<T>> &root ,T&& value);
        decltype(auto) find_smallest_sub_right_child(std::shared_ptr<Node<T>> &root);
        decltype(auto) remove(T&& value);
        std::shared_ptr<Node<T>> remove_node(std::shared_ptr<Node<T>> &root, T&& value);
        decltype(auto) inorder();
        decltype(auto) print_inorder(std::shared_ptr<Node<T>> &root);
};

template<typename T>
Bst<T>::Bst() : root(nullptr)
{

}

template<typename T>
decltype(auto) Bst<T>::search(T&& value)
{
    return search_node(root, std::forward<T>(value));
}

template<typename T>
decltype(auto) Bst<T>::search_node(std::shared_ptr<Node<T>> &root, T&& value)
{
    if (root->value == value)
        return true;

    if (root->left == nullptr && root->right == nullptr && root->value != value)
        return false;

    if (root->value < value) {
        return search_node(root->right, std::forward<T>(value));
    }

    if (root->value > value) {
        return search_node(root->left, std::forward<T>(value));
    }

    return false;
}

template<typename T>
decltype(auto) Bst<T>::insert(T&& value)
{
    root = insert_node((root), std::forward<T>(value));
}

template<typename T>
decltype(auto) Bst<T>::insert_node(std::shared_ptr<Node<T>> &root, T&& value)
{
    if (root == nullptr) {
        root = std::make_shared<Node<T>>(std::forward<T>(value));
        return root;
    }

    if (value < root->value) {
        root->left = (insert_node(root->left, std::forward<T>(value)));
    }
    if (value > root->value) 
        root->right = (insert_node(root->right, std::forward<T>(value)));

    return root;
}

template<typename T>
decltype(auto) Bst<T>::remove(T&& value)
{
    root = remove_node(root, std::forward<T>(value));
}

template<typename T>
std::shared_ptr<Node<T>> Bst<T>::remove_node(std::shared_ptr<Node<T>> &root, T&& value)
{
    if (root == nullptr)
        return root;

    if (value < root->value)
        root->left = remove_node(root->left, std::forward<T>(value));
    else if (value > root->value) 
        root->right = remove_node(root->right, std::forward<T>(value));
    else {
        // if (root->left == nullptr && root->right == nullptr)
        //     return root;
        if (root->left == nullptr) {
            return root->right;
        } 
        else if (root->right == nullptr) {
            return root->left;
        }
        else {
            root->value = find_smallest_sub_right_child(root->right);
            root->right = remove_node(root->right, std::forward<T>(root->value));
        }
    }
    return root; 
}

template<typename T>
decltype(auto) Bst<T>::find_smallest_sub_right_child(std::shared_ptr<Node<T>> &root)
{
    auto traverse = root;
    auto minv = root->value;
    while (traverse->left != NULL) {
        minv = traverse->left->value;
        traverse = traverse->left;
    }
    return minv;
}

template<typename T>
decltype(auto) Bst<T>::inorder()
{
    print_inorder(root);
}

template<typename T>
decltype(auto) Bst<T>::print_inorder(std::shared_ptr<Node<T>> &root)
{
    if (root == nullptr) return;

    print_inorder(root->left);
    std::cout << root->value << std::endl;
    print_inorder(root->right);
}

int main()
{
    Bst<int> bst;
    bst.insert(10);
    bst.insert(20);
    bst.insert(1);
    bst.insert(54);
    bst.insert(6);

    bst.inorder();

    if (bst.search(4))  std::cout << "Found!!!" << std::endl;
    else std::cout << "Not Found!!!" << std::endl;
    if (bst.search(3)) std::cout << "Found!!!" << std::endl;
    else std::cout << "Not Found!!!" << std::endl;
    
    bst.remove(10);
    
    bst.inorder();
    return 0;
}