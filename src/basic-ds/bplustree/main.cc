#include "bplustree.h"

#include <iostream>

int main() {
    // BPlusTree *tree = new BPlusTree(3);

    // tree->insert(1);
    // tree->insert(6);
    // tree->insert(8);
    // tree->insert(9);
    // tree->insert(3);
    // tree->insert(2);
    // tree->insert(7);
    // tree->insert(30);
    // tree->insert(35);
    // tree->insert(39);
    // tree->insert(40);
    // tree->insert(50);
    // tree->insert(55);
    // tree->insert(62);
    // tree->insert(60);
    // tree->insert(77);
    // tree->insert(80);
    // tree->insert(90);
    // tree->insert(61);
    // tree->insert(68);
    // tree->insert(69);
    // tree->insert(95);
    // tree->insert(100);
    // tree->insert(103);
    // tree->insert(107);
    // tree->insert(111);
    // tree->insert(115);
    // tree->insert(120);

    BPlusTree *tree = new BPlusTree(6);

    tree->insert(400);
    tree->insert(600);
    tree->insert(700);
    tree->insert(800);
    tree->insert(900);
    tree->insert(1000);
    tree->insert(756);
    tree->insert(666);
    tree->insert(333);
    tree->insert(100);
    tree->insert(200);
    tree->insert(300);
    tree->insert(500);
    tree->insert(555);
    tree->insert(580);
    tree->insert(33);
    tree->insert(66);
    tree->insert(99);
    tree->insert(130);
    tree->insert(180);
    tree->insert(250);

    tree->delete_node(66);
    tree->delete_node(100);
    tree->delete_node(180);
    tree->delete_node(250);
    tree->delete_node(99);
    tree->delete_node(130);
    // tree->delete_node(300);

    tree->print_tree();

    // tree->delete_node(400);
    // tree->delete_node(600);
    // tree->delete_node(700);
    // tree->delete_node(800);
    // tree->delete_node(900);
    // tree->delete_node(1000);
    // tree->delete_node(756);
    // tree->delete_node(666);
    // tree->delete_node(333);
    // tree->delete_node(100);
    // tree->delete_node(200);
    // tree->delete_node(300);
    // tree->delete_node(500);
    // tree->delete_node(555);
    // tree->delete_node(580);
    // tree->delete_node(33);
    // tree->delete_node(66);
    // tree->delete_node(99);
    // tree->delete_node(130);
    // tree->delete_node(180);
    // tree->delete_node(250);



    return 0;
}