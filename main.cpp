#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>

using namespace std;

template <typename t, int order>
class Node {
private:
    t keys[order-1];
    Node<t, order>* children[order];
    int numberOfKeys;
    bool isLeaf;

public:
    Node(bool leaf) {
        isLeaf = leaf;
        numberOfKeys = 0;
        for (int i = 0; i < order; i++)
            children[i] = nullptr;
    }

    template<typename T, int ORDER>
    friend class BTree;
};

template <typename t, int order>
class BTree {
private:
    Node<t, order>* root;
public:
    BTree() {
        root = nullptr;
    }
};


int main() {

    return 0;
}