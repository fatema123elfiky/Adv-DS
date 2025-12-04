#include <iostream>
#include <queue>

using namespace std;

template <typename t, int order>
class Node {

public:
    // minor change for intermediate step
    t keys[order];
    Node<t, order>* children[order+1];

    int numberOfKeys;
    bool isLeaf;
    Node<t,order>* parent;
    Node(bool leaf) {
        isLeaf = leaf;
        numberOfKeys = 0;
        for (int i = 0; i < order+1; i++)
            children[i] = nullptr;
        parent = nullptr;
    }


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
    int shifting (Node <t,order> * node , t val){
        node->numberOfKeys++;
        int idx;
        for (idx = 0; idx < node->numberOfKeys-1 ; ++idx)
            if(node->keys[idx] >val)
                break;
        t tempVal ;
        for (int i = idx; i < node->numberOfKeys-1 ; ++i) {
            tempVal = node -> keys[i];
            node->keys[i]=val;
            val = tempVal;
        }
        node->keys[node->numberOfKeys-1]=val;
        return idx;

    }

    void split (Node<t,order> * temp){
            if(temp->numberOfKeys!=order)
                return;
            //split !
            t pastKeys [order];
            for (int idx = 0; idx < order ; ++idx) {
                pastKeys[idx]= temp->keys[idx];
            }
            t mid = pastKeys[order/2];

            //splited node
            Node<t,order> * left = new Node<t, order>(true);
            Node<t,order> * right = new Node<t,order>(true);
            for (int idx = 0; idx < order; ++idx) {
                if (idx < order / 2)
                    left->keys[idx] = pastKeys[idx] , left->numberOfKeys++;
                else if (idx > order / 2)
                    right->keys[idx] = pastKeys[idx], right->numberOfKeys++;
            }

            //Two cases for split :
            // If there is no parent
            if(!temp->parent){

                temp->isLeaf = false, temp->numberOfKeys = 1, temp->keys[0] = mid;
                temp->children[0] = left, temp->children[1] = right;
                left->parent=temp , right->parent=temp;

            }else{//If there is a parent

                Node<t,order> * par = temp->parent;
                int index = shifting(par,mid);

                Node<t,order> * tempVal1 =par->children[index] , *tempVal2 = par->children[index+1];
                par->children[index++]=left;
                par->children[index++]=right;
                for (int i = index; i < par->numberOfKeys+1; i+=2) {
                    Node<t,order> * val1 = par->children[i] , *val2=par->children[i+1];
                    par->children[i]=tempVal1 , par->children[i+1]=tempVal2;
                    tempVal1=val1,tempVal2=val2;
                }
                left->parent=right->parent=par;
                delete temp;
                split(par);

            }
    }

public:
    BTree() {
        root = nullptr;
    }
    void Print() {
        dfs(root);
    }

    void Insert(t val){
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


        //Insert anyway
        shifting(temp,val);

        // If we reached max
        // so, no leaf , numberOfKeys will change
        // parent will change ,keys
        if(temp->numberOfKeys == order) {
            split(temp);
            //cout<<"Split\n";
        }
        //cout<<"insertion done\n";



        }

    void test (){

        queue<Node<t,order> *> bfs;
        bfs.push(root);
        while (!bfs.empty()){
            auto x = bfs.front();
            bfs.pop();
            for (int i = 0; i < x->numberOfKeys; ++i)
                cout<<x->keys[i]<<' ';

            cout<<'\n';
            for (int i = 0; i < x->numberOfKeys+1&& x->children[i]->isLeaf; ++i)
                bfs.push(x->children[i]);


        }
    }

};


int main() {
    // Construct a BTree of order 3, which stores int data
    BTree<int,3> t1;

    t1.Insert(1);
    t1.Insert(5);
    t1.Insert(0);
    t1.Insert(4);
    t1.Insert(3);
    t1.Insert(2);

    //t1.test();
    //t1.Print(); // Should output the following on the screen:

    /*
    1,4
      0
      2,3
      5
    */

    // Construct a BTree of order 5, which stores char data
    BTree<char,5> t;

    // Look at the example in our lecture:
    t.Insert('G');
    t.Insert('I');
    t.Insert('B');
    t.Insert('J');
    t.Insert('C');
    t.Insert('A');
    t.Insert('K');
    t.Insert('E');
    t.Insert('D');
    t.Insert('S');
    t.Insert('T');
    t.Insert('R');
    t.Insert('L');
    t.Insert('F');
    t.Insert('H');
    t.Insert('M');
    t.Insert('N');
    t.Insert('P');
    t.Insert('Q');

    //t.test();
    //t.Print(); // Should output the following on the screen:

    /*
    K
      C,G
        A,B
        D,E,F
        H,I,J
      N,R
        L,M
        P,Q
        S,T
    */
    return 0;
}