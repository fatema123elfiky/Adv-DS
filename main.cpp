#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>

using namespace std;

template <typename t, int order>
class Node {
private:
    t keys[order]; // minor change for intermediate step
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
    void shifting (Node <t,order> * node , t val){
        node->numberOfKeys++;
        int idx;
        for (idx = 0; idx < node->numberOfKeys; ++idx)
            if(node->keys[idx] >val)
                break;
        t tempVal ;
        for (int i = idx; i < node->numberOfKeys ; ++i) {
            tempVal = node -> keys[idx];
            node->keys[idx]=val;
            val = tempVal;
        }

    }

public:
    BTree() {
        root = nullptr;
    }
    void Print() {
        dfs(root);
    }

    void insert(t val){
        //First insertion
        if(!root) {
            root = new Node<t, order>(true), root->keys[0] = val;
            return;
        }

        //Search first for the right position
        Node <t,order> * temp = root;

        while (!temp && !temp->isLeaf){

            //iterating inside the fatty node
            bool done = false;
            for (int idx = 0 ; idx < temp->numberOfKeys; ++idx) {
                if(temp->keys[idx] > val){
                    temp= temp->children[idx];
                    done=true;
                    break;
                }
            }
            if(!done)
               temp=temp->children[temp->numberOfKeys];


        }


        // If insertion --> no max (safe)
        if(temp->numberOfKeys!= order)
            shifting(temp,val);
        else{
            // If insertion --> yes max
            // it became not leaf , numberOfKeys became one
            //
            temp->isLeaf=false;

            //split !
            int mid = order /2;
            t pastKeys [order] = temp->keys;
            temp->keys[0] = pastKeys[mid];



        }

    }
};


int main() {

    return 0;
}
