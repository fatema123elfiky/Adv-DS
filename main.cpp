#include <iostream>

using namespace std;

template <typename T, int order>
class Node
{
public:
    // array of keys
    T * keys;
    // array of pointers to the children nodes
    Node<T, order>** children;
    // pointer to the parent
    Node<T,order>* parent;
    // current number of keys in the node
    int numberOfKeys;
    // if the node is leaf or internal node
    bool isLeaf;

    // Constructor of Node
    Node(bool leaf)
    {
        keys = new T[order];
        children = new Node<T, order> * [order+1];
        isLeaf = leaf;
        numberOfKeys = 0;
        // initialize all children to nullptr
        for (int i = 0; i < order+1; i++)
            children[i] = nullptr;
        parent = nullptr;
    }

    // Destructor of Node to delete pointers
    ~Node()
    {
        // delete each pointer in the children array
        for (int i = 0; i < numberOfKeys + 1; i++)
            delete children[i];
        delete[] keys;
        delete[] children;
    }
};

template <typename T, int order>
class BTree
{
private:
    // pointer to the root of the tree
    Node<T, order>* root;

    // Function to print the B-tree using depth-first-search
    void dfs(Node<T, order>* node, int depth = 0)
    {
        // base case: if current node is null stop recursion
        if (!node) return;

        // print spaces according to the level of the node
        for (int i = 0; i < 2*depth; i++)
            cout << " ";

        // print all keys in the current node
        for (int i = 0; i < node->numberOfKeys; i++)
        {
            cout << node->keys[i];
            // print commas between internal keys
            cout << (i==(node->numberOfKeys-1) ? "" : ",");
        }
        cout << "\n";

        // if the current node has more children: traverse its children
        if (!node->isLeaf)
        {
            for (int i = 0; i <= node->numberOfKeys; i++)
            {
                if (node->children[i])
                    dfs(node->children[i], depth+1);
            }
        }
    }

    // Function to insert new key to the array of keys and maintain sorted order
    int shifting (Node <T,order> * node , T val)
    {
        // increase number of keys to put the new key
        node->numberOfKeys++;

        // search for the suitable space to insert val in it
        int idx;
        while (idx < node->numberOfKeys-1)
        {
            // break if current key greater than val
            if(node->keys[idx] >val)
                break;
            idx++;
        }
        // variable to save the key that will be shifted
        T tempVal;
        for (int i = idx; i < node->numberOfKeys-1; ++i)
        {
            // shift the current key and put the val in cur position
            tempVal = node -> keys[i];
            node->keys[i]=val;
            val = tempVal;
        }
        // put the greatest value in the last position
        node->keys[node->numberOfKeys-1]=val;
        return idx;
    }

    void shifting_child (Node<T,order> * par , Node<T,order> * right , int index){
        Node<T,order> * val1 =right ;
        Node<T,order> * tempVal1 ;
        for (int i = index; i < par->numberOfKeys+1; i++)
        {
            tempVal1 =par->children[index] ;
            par->children[i]=val1;
            val1=tempVal1;
        }
    }

    void children_handling (Node<T,order> ** pastChildren, Node<T,order> *left ,Node<T,order> *right){
        // children handle
        if(pastChildren[0])
        {
            int i;
            for ( i = 0; i < (order+1)/2 ; ++i)
                left->children[i]=pastChildren[i];

            for (int j = i; j < (order+1); ++j)
                right->children[j-i]=pastChildren[j];

        }
    }
    void split (Node<T,order> * temp,bool leaf = true){
            if(temp->numberOfKeys!=order)
                return;

            // before changes
            T pastKeys [order];
            for (int idx = 0; idx < order ; ++idx)
                pastKeys[idx]= temp->keys[idx];

            Node<T,order> * pastChildren [order+1] ;
            for (int i = 0; i < order+1; ++i)
                pastChildren[i]= temp->children[i];


            //split !
            T mid = pastKeys[order/2];
            Node<T,order> * left = new Node<T, order>(leaf);
            Node<T,order> * right = new Node<T,order>(leaf);
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

                Node<T,order> * par = temp->parent;
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
    // Constructor for the BTree
    BTree()
    {
        // initialize the root to nullptr
        root = nullptr;
    }

    // Function to print nodes in the tree
    void Print()
    {
        // print using depth-first-search
        dfs(root);
    }

    // Function to find which leaf node a value should be inserted in
    Node<T,order>* Search (Node<T,order>* node, T val)
    {
        // node to using in traversing
        Node <T,order> * temp = node;

        // while temp is an internal node continue until you reach a leaf
        while (temp && !temp->isLeaf)
        {
            // boolean to check if val is smaller than any key
            bool found = false;
            // iterate through keys in the current node
            for (int idx = 0 ; idx < temp->numberOfKeys; ++idx)
            {
                // if current key is greater than val
                if(temp->keys[idx] > val)
                {
                    // val should be a left child for this key
                    temp= temp->children[idx];
                    found=true;
                    break;
                }
            }
            // go to the right most child of this node
            if(!found)
                temp=temp->children[temp->numberOfKeys];
        }
        // return pointer to the correct leaf
        return temp;
    }

    // Function to insert a new value to the BTree
    void Insert(T val)
    {
        // If the tree is empty
        if(!root) {
            // Create root node
            root = new Node<T, order>(true);
            root->keys[0] = val ;
            root->numberOfKeys++;
            return;
        }

        // Search for the right leaf to insert in it
        Node <T,order> * leafPos = Search(root, val);

        // Insert val in the correct leaf
        shifting(leafPos,val);

        // If the leaf to insert in is full
        if(leafPos->numberOfKeys == order)
            // split this leaf
            split(leafPos);
    }

    // Destructor for the B-tree
    ~BTree()
    {
        // Delete the root pointer
        delete root;
    }
};

int main()
{
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

    return 0;
}