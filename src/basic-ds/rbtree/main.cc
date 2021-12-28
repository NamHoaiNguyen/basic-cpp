#include "rbtree.h"

#include <memory>

int main() {
    // auto rb = std::make_unique<RBTree>();
    RBTree *rb = new RBTree();
    int arr_test[] = {1,4,6,3,5,7,8,2,9}; 
    // {1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (const auto& e : arr_test) {
        rb->insert(e);
        rb->inorder();
        std::cout << std::endl;
    }

    // rb->inorder();

    return 0;

}