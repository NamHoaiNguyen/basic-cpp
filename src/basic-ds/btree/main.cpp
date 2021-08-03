#include "btree.h"
#include "btree_node.h"

#include <iostream>
#include <memory>

int main()
{
    std::shared_ptr<BtreeNode<int, int, bool>> node = std::make_shared<BtreeNode<int, int, bool>>( 5, false );
    BTree<int, int, bool> t(3);
    t.insert_node(10);
    t.insert_node(20);
    t.insert_node(5);
    t.insert_node(6);
    t.insert_node(12);
    t.insert_node(30);
    t.insert_node(7);
    t.insert_node(17);
  
    std::cout << "Traversal of the constucted tree is ";
    t.traverse();  
    return 0;
}
