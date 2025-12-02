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

    void dfs(Node<t, order>* node, int depth = 0) {
        if (!node) return;

        for (int i = 0; i < 2*depth; i++) {
            cout << " ";
        }
        
        for (int i = 0; i < node->numberOfKeys; i++) {
            cout << node->keys[i];
            cout << (i==(node->numberOfKeys-1)? "" : ",");
        }
        cout << "\n";

        if (!node->isLeaf) {
            for (int i = 0; i <= node->numberOfKeys; i++) {
                if (node->children[i])
                    dfs(node->children[i], depth+1);
            }
        }
    }

    void Print() {
        dfs(root);
    }
};


int main() {

    return 0;
}
