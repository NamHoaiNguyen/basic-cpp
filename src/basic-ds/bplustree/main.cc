#include "bplustree.h"

#include <iostream>

int main() {
    BPlusTree *tree = new BPlusTree(5);

    tree->insert(1);
    tree->insert(6);
    tree->insert(8);
    tree->insert(9);
    tree->insert(3);
    tree->insert(2);
    tree->insert(7);
    tree->insert(30);
    tree->insert(35);
    tree->insert(39);
    tree->insert(40);
    tree->insert(50);
    tree->insert(55);
    tree->insert(62);
    tree->insert(60);
    tree->insert(77);

    tree->print_tree();

    return 0;
}