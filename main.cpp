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
        numbrOfKeys = 0;
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

            // before changes
            t pastKeys [order];
            for (int idx = 0; idx < order ; ++idx)
                pastKeys[idx]= temp->keys[idx];

            Node<t,order> * pastChildren [order+1] ;
            for (int i = 0; i < order+1; ++i)
                pastChildren[i]= temp->children[i];


            //split !
            t mid = pastKeys[order/2];
            Node<t,order> * left = new Node<t, order>(true);
            Node<t,order> * right = new Node<t,order>(true);
            for (int idx = 0 ,idx2=0; idx < order; ++idx) {
                if (idx < order / 2)
                    left->keys[idx] = pastKeys[idx] , left->numberOfKeys++,idx2++;
                else if (idx > order / 2)
                    right->keys[idx-idx2] = pastKeys[idx], right->numberOfKeys++;
                else
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

                // children handle
                if(pastChildren[0]){
                    int i;
                    for ( i = 0; i < (order+1)/2 ; ++i)
                        left->children[i]=pastChildren[i];

                    for (int j = i; j < (order+1); ++j)
                        right->children[j-i]=pastChildren[j];

                }

            }else{//If there is a parent

                Node<t,order> * par = temp->parent;
                int index = shifting(par,mid);



                par->children[index++]=left;


                Node<t,order> * val1 =right ;
                Node<t,order> * tempVal1 ;
                for (int i = index; i < par->numberOfKeys+1; i++) {
                    tempVal1 =par->children[index] ;
                    par->children[i]=val1;
                    val1=tempVal1;
                }
                left->parent=right->parent=par;
                // children handle
                if(pastChildren[0]){
                    int i;
                    for ( i = 0; i < (order+1)/2 ; ++i)
                        left->children[i]=pastChildren[i];

                    for (int j = i; j < (order+1); ++j)
                        right->children[j-i]=pastChildren[j];

                }
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
            root = new Node<t, order>(true), root->keys[0] = val , root->numberOfKeys++;
            return;
        }

        //Search first for the right position
        Node <t,order> * temp = root;

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
            for (int i = 0; i < x->numberOfKeys+1&& x->children[i]; ++i)
                bfs.push(x->children[i]);


        }
    }

};

void test1(){
    // Construct a BTree of order 3, which stores int data
    BTree<int,3> t1;

    t1.Insert(1);
    t1.Insert(5);
    t1.Insert(0);
    t1.Insert(4);
    t1.Insert(3);
    t1.Insert(2);
    t1.Print(); // Should output the following on the screen:

   //t1.test();
    /*
    1,4
      0
      2,3
      5
    */

}

void test2(){

    // Construct a BTree of order 5, which stores char data
    BTree<char,5> t;

    // Look at the example in our lecture:
    t.Insert('G');
    cout<<"passed 1\n";
    t.Insert('I');
    cout<<"passed 2\n";
    t.Insert('B');
    cout<<"passed 3\n";
    t.Insert('J');
    cout<<"passed 4\n";
    t.Insert('C');
    cout<<"passed 5\n";
    t.Insert('A');
    cout<<"passed 6\n";
    t.Insert('K');
    cout<<"passed 7\n";
    t.Insert('E');
    cout<<"passed 8\n";
    t.Insert('D');
    cout<<"passed 9\n";
    t.Insert('S');
    cout<<"passed 10\n";
    t.Insert('T');
    cout<<"passed 11\n";
    t.Insert('R');
    cout<<"passed 12\n";
    t.Insert('L');
    cout<<"passed 13\n";
    t.Insert('F');
    cout<<"passed 14\n";
    t.Insert('H');
    cout<<"passed 15\n";
    t.Insert('M');
    cout<<"passed 16\n";
    t.Insert('N');
    cout<<"passed 17\n";
    t.Insert('P');
    cout<<"passed 18\n";
    t.Insert('Q');
    cout<<"passed 19\n";


    //t.test();
    t.Print();

}

int main() {

    test1();
    test2();

    return 0;
}
