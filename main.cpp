#include <iostream>


using namespace std;




template <typename t, int order>
class Node {

public:
    // minor change for intermediate step
    t * keys;
    Node<t, order>** children ;

    int numberOfKeys;
    bool isLeaf;
    Node<t,order>* parent;
    Node(bool leaf) {
        keys = new t[order];
        children = new Node<t, order> * [order+1];
        isLeaf = leaf;
        numberOfKeys = 0;
        for (int i = 0; i < order+1; i++)
            children[i] = nullptr;
        parent = nullptr;
    }
    ~Node(){
        for (int i = 0; i < numberOfKeys + 1; i++)
            delete children[i];


        delete[] keys;
        delete[] children;


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

    void shifting_child (Node<t,order> * par , Node<t,order> * right , int index){
        Node<t,order> * val1 =right ;
        Node<t,order> * tempVal1 ;
        for (int i = index; i < par->numberOfKeys+1; i++) {
            tempVal1 =par->children[index] ;
            par->children[i]=val1;
            val1=tempVal1;
        }
    }

    void children_handling (Node<t,order> ** pastChildren, Node<t,order> *left ,Node<t,order> *right){
        // children handle
        if(pastChildren[0]){
            int i;
            for ( i = 0; i < (order+1)/2 ; ++i)
                left->children[i]=pastChildren[i];

            for (int j = i; j < (order+1); ++j)
                right->children[j-i]=pastChildren[j];

        }
    }
    void split (Node<t,order> * temp,bool leaf = true){
            if(temp->numberOfKeys!=order)
                return;

            // before changes
            t pastKeys [order];
            for (int idx = 0; idx < order ; ++idx)
                pastKeys[idx]= temp->keys[idx];

            Node<t,order> * pastChildren [order+1] ;
            for (int i = 0; i < order+1; ++i)
                pastChildren[i]= temp->children[i];


            //split !
            t mid = pastKeys[order/2];
            Node<t,order> * left = new Node<t, order>(leaf);
            Node<t,order> * right = new Node<t,order>(leaf);
            for (int idx = 0 ,idx2=0; idx < order; ++idx) {
                if (idx < order / 2) {
                    left->keys[idx] = pastKeys[idx];
                    left->numberOfKeys++, idx2++;
                }else if (idx > order / 2) {
                    right->keys[idx - idx2] = pastKeys[idx];
                    right->numberOfKeys++;
                }else
                    idx2++;
            }



            //Two cases for split :
            // If there is no parent
            if(!temp->parent){

                // changes
                temp->isLeaf = false;
                temp->numberOfKeys = 1;
                temp->keys[0] = mid;

                temp->children[0] = left, temp->children[1] = right;
                left->parent=temp ;
                right->parent=temp;

                children_handling(pastChildren,left,right);


            }else{//If there is a parent

                Node<t,order> * par = temp->parent;
                int index = shifting(par,mid);



                par->children[index++]=left;
                shifting_child(par,right,index);


                left->parent=right->parent=par;
                children_handling(pastChildren,left,right);
                delete temp;
                split(par,false);

            }
    }

public:
    ~BTree(){
        delete root;
    }
    BTree() {
        root = nullptr;
    }
    void Print() {
        dfs(root);
    }

    Node<t,order> * Search (Node<t,order> * node , t val ){
        Node <t,order> * temp = node;

        while (temp && !temp->isLeaf){
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
        return temp;
    }

    void Insert(t val){
        //First insertion
        if(!root) {
            root = new Node<t, order>(true);
            root->keys[0] = val ;
            root->numberOfKeys++;
            return;
        }

        //Search first for the right position
        Node <t,order> * temp = Search(root , val);

        //Insert anyway
        shifting(temp,val);

        // If we reached max
        if(temp->numberOfKeys == order)
            split(temp);


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
    t1.Print(); // Should output the following on the screen:

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


    t.Print();

    BTree<int,4> f;
    f.Insert(5);
    f.Insert(3);
    f.Insert(21);
    f.Insert(9);
    f.Insert(1);
    f.Insert(13);
    f.Insert(2);
    f.Insert(7);

    f.Print();

    BTree<int,5>tt;
    tt.Insert(1);
    tt.Insert(2);
    tt.Insert(3);
    tt.Insert(4);
    tt.Insert(5);
    tt.Insert(6);
    tt.Insert(7);
    tt.Insert(8);
    tt.Insert(9);
    tt.Insert(10);
    tt.Insert(11);
    tt.Insert(12);
    tt.Insert(13);
    tt.Insert(14);
    tt.Insert(15);
    tt.Insert(16);
    tt.Insert(17);
    tt.Insert(18);
    tt.Insert(19);
    tt.Insert(20);
    tt.Print();
    return 0;
}