#include "btree.h"
#include "btree_node.h"

#include <iostream>
#include <memory>

int main()
{
    std::shared_ptr<BtreeNode<int, int>> node = std::make_shared<BtreeNode<int, int>>( 5, false );
    BTree<int, int> btree(3);
    
    return 0;
}
