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
        std::unique_ptr<Node<T>> left;
        std::unique_ptr<Node<T>> right;
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
        std::unique_ptr<Node<T>> root;
    public:
        // std::unique_ptr<Node<T>> root;

        Bst();
        decltype(auto) insert(T&& value);
        decltype(auto) insert_node(std::unique_ptr<Node<T>> &root ,T&& value);
        decltype(auto) inorder();
        decltype(auto) print_inorder(std::unique_ptr<Node<T>> &root);
};

template<typename T>
Bst<T>::Bst() : root(nullptr)
{

}

template<typename T>
decltype(auto) Bst<T>::insert(T&& value)
{
    insert_node((root), std::forward<T>(value));
}

template<typename T>
decltype(auto) Bst<T>::insert_node(std::unique_ptr<Node<T>> &root, T&& value)
{
    if (root == nullptr) 
        root = std::make_unique<Node<T>>(std::forward<T>(value));
        return root;
    
    if (value < root->value) {
        Node<T> *temp = root.get();
        temp->left = std::move(insert_node(root->left, std::forward<T>(value)));
    }
    if (value > root->value) {
        Node<T> *temp = root.get();
        temp->right = std::move(insert_node((root.get()->right), std::forward<T>(value)));
    }
    // Node<T> *temp = root.get();
    // Node<T> *prev = root.get();
    
    // while (temp != nullptr) {
    //     prev = temp;
    //     if (temp->value < value)
    //         temp = temp->right.get();
    //     else
    //         temp = temp->left.get();
    // }
    // if (prev->value < value) {
    //     prev->right = std::move(value);
    // } else {
    //     prev->left = std::move(value);
    // }

    return root;
}

template<typename T>
decltype(auto) Bst<T>::inorder()
{
    print_inorder(root);
}

template<typename T>
decltype(auto) Bst<T>::print_inorder(std::unique_ptr<Node<T>> &root)
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
 //   bst.insert(20);
    bst.insert(4);
    bst.inorder();
    return 0;
}