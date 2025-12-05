// Ahmed Noureldin ID: 20230035
// Fatema Elzhraa Ahmed ID: 20230280
// Salsabil Bahaa Eldin ID: 20230586

#include <iostream>

using namespace std;

template<typename T, int order>
class Node
{
public:
    // array of keys
    T *keys;
    // array of pointers to the children nodes
    Node<T, order> **children;
    // pointer to the parent
    Node<T, order> *parent;
    // current number of keys in the node
    int numberOfKeys;
    // if the node is leaf or internal node
    bool isLeaf;

    // Constructor of Node
    Node(bool leaf)
    {
        keys = new T[order];
        children = new Node<T, order> *[order + 1];
        isLeaf = leaf;
        numberOfKeys = 0;
        // initialize all children to nullptr
        for (int i = 0; i < order + 1; i++)
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

template<typename T, int order>
class BTree
{
private:
    // pointer to the root of the tree
    Node<T, order> *root;

    // Function to print the B-tree using depth-first-search
    void dfs(Node<T, order> *node, int depth = 0)
    {
        // base case: if current node is null stop recursion
        if (!node) return;

        // print spaces according to the level of the node
        for (int i = 0; i < 2 * depth; i++)
            cout << " ";

        // print all keys in the current node
        for (int i = 0; i < node->numberOfKeys; i++)
        {
            cout << node->keys[i];
            // print commas between internal keys
            cout << (i == (node->numberOfKeys - 1) ? "" : ",");
        }
        cout << "\n";

        // if the current node has more children: traverse its children
        if (!node->isLeaf)
        {
            for (int i = 0; i <= node->numberOfKeys; i++)
            {
                if (node->children[i])
                    dfs(node->children[i], depth + 1);
            }
        }
    }

    // Function to insert new key to the array of keys and maintain sorted order
    int shifting(Node<T, order> *node, T val)
    {
        // increase number of keys to put the new key
        node->numberOfKeys++;

        // search for the suitable space to insert val in it
        int idx = 0;
        while (idx < node->numberOfKeys - 1)
        {
            // break if current key greater than val
            if (node->keys[idx] > val)
                break;
            idx++;
        }
        // variable to save the key that will be shifted
        T tempVal;
        for (int i = idx; i < node->numberOfKeys - 1; ++i)
        {
            // shift the current key and put the val in cur position
            tempVal = node->keys[i];
            node->keys[i] = val;
            val = tempVal;
        }
        // put the greatest value in the last position
        node->keys[node->numberOfKeys - 1] = val;
        return idx;
    }

    // Function to insert new children pointers inside parent
    void shifting_child(Node<T, order> *par, Node<T, order> *right, int index)
    {
        // pointers to traverse the children and do shifting
        Node<T, order> *val1 = right;
        Node<T, order> *tempVal1;
        // shifts the children after the inserted parent by one position to put the right child
        for (int i = index; i < par->numberOfKeys + 1; i++)
        {
            tempVal1 = par->children[i];
            par->children[i] = val1;
            val1 = tempVal1;
        }
    }

    // Function to reassign old children to the new left and right nodes
    void children_handling(Node<T, order> **pastChildren, Node<T, order> *left, Node<T, order> *right)
    {
        // if there were children split them
        if (pastChildren[0])
        {
            int i;
            // put the first half of children in the left node
            for (i = 0; i < (order + 1) / 2; ++i)
            {
                left->children[i] = pastChildren[i];
                // set the parent of the child
                if (left->children[i]) left->children[i]->parent = left;
            }
            // if order is even, add one more child in the left child (right bias)
            if ((order + 1) % 2)
            {
                left->children[i] = pastChildren[i];
                // set the parent of the child
                if (left->children[i]) left->children[i]->parent = left;
                i++;
            }
            // put the next half of children in the right node
            for (int j = i; j < (order + 1); ++j)
            {
                right->children[j - i] = pastChildren[j];
                // set the parent of the child
                if (right->children[j - i]) right->children[j - i]->parent = right;
            }
        }
    }

    // Function to split the full node
    void split(Node<T, order> *node, bool leaf = true)
    {
        // Check if we need split at current node
        if (node->numberOfKeys != order)
            return;

        // store keys before deleting the node
        T *pastKeys = new T[order];
        for (int idx = 0; idx < order; ++idx)
            pastKeys[idx] = node->keys[idx];

        // store children before deleting the node
        Node<T, order> **pastChildren = new Node<T, order> *[order + 1];
        for (int i = 0; i < order + 1; ++i)
            pastChildren[i] = node->children[i];

        // Get the middle key to split from it
        T mid = pastKeys[order / 2];

        // Create 2 new nodes
        Node<T, order> *left = new Node<T, order>(leaf);
        Node<T, order> *right = new Node<T, order>(leaf);

        // Distribute the key to the left and right children
        int idx2 = 0;
        for (int idx = 0; idx < order; ++idx)
        {
            // Left part
            if (idx < order / 2)
            {
                left->keys[idx] = pastKeys[idx];
                left->numberOfKeys++, idx2++;
            }
            // Right part (the middle element is skipped)
            else if (idx > order / 2)
            {
                right->keys[idx - idx2] = pastKeys[idx];
                right->numberOfKeys++;
            } else
                idx2++;
        }

        // Two cases for split:
        // Case 1: If there is no parent
        if (!node->parent)
        {
            // Make the old node the new root
            node->isLeaf = false;
            node->numberOfKeys = 1;
            node->keys[0] = mid;

            // Attach the left and right children to it
            node->children[0] = left;
            node->children[1] = right;
            left->parent = node;
            right->parent = node;

            // Redistribute children if internal node
            children_handling(pastChildren, left, right);
        } else //If there is a parent
        {
            // Insert middle key into the parent
            Node<T, order> *par = node->parent;
            // get the index where the middle element is inserted
            int index = shifting(par, mid);
            // put left child at index
            par->children[index++] = left;
            // put right child after it
            shifting_child(par, right, index);
            // set the parent of left and right children
            left->parent = right->parent = par;
            // Reassign children to left & right
            children_handling(pastChildren, left, right);

            // Clear old children pointers
            for (int i = 0; i < order + 1; i++)
                node->children[i] = nullptr;

            // delete the old node
            delete node;

            // Recursively split parent if it is full
            split(par, false);
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
    Node<T, order> *Search(Node<T, order> *node, T val)
    {
        // node to using in traversing
        Node<T, order> *temp = node;

        // while temp is an internal node continue until you reach a leaf
        while (temp && !temp->isLeaf)
        {
            // boolean to check if val is smaller than any key
            bool found = false;
            // iterate through keys in the current node
            for (int idx = 0; idx < temp->numberOfKeys; ++idx)
            {
                // if current key is greater than val
                if (temp->keys[idx] > val)
                {
                    // val should be a left child for this key
                    temp = temp->children[idx];
                    found = true;
                    break;
                }
            }
            // go to the right most child of this node
            if (!found)
                temp = temp->children[temp->numberOfKeys];
        }
        // return pointer to the correct leaf
        return temp;
    }

    // Function to insert a new value to the BTree
    void Insert(T val)
    {
        // If the tree is empty
        if (!root)
        {
            // Create root node
            root = new Node<T, order>(true);
            root->keys[0] = val;
            root->numberOfKeys++;
            return;
        }

        // Search for the right leaf to insert in it
        Node<T, order> *leafPos = Search(root, val);

        // Insert val in the correct leaf
        shifting(leafPos, val);

        // If the leaf to insert in is full
        if (leafPos->numberOfKeys == order)
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
    cout << "Test 1, order 3" << endl;
    // Construct a BTree of order 3, which stores int data
    BTree<int,3> t1;

    t1.Insert(1);
    t1.Insert(5);
    t1.Insert(0);
    t1.Insert(4);
    t1.Insert(3);
    t1.Insert(2);

    t1.Print(); // Should output the following on the screen:

    cout << "\nTest 2, order 5" << endl;
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

    cout << "\nTest 3, order 4" << endl;
    BTree<int,4> f;
    f.Insert(5);
    f.Insert(3);
    f.Insert(21);
    f.Insert(9);
    f.Insert(1);
    f.Insert(13);
    f.Insert(2);
    f.Insert(7);
    f.Insert(10);
    f.Insert(12);
    f.Insert(4);
    f.Insert(8);


    f.Print();

    cout << "\nTest 4, order 5" << endl;
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
    tt.Insert(21);
    tt.Insert(22);
    tt.Insert(23);
    tt.Insert(24);
    tt.Insert(25);
    tt.Insert(26);

    tt.Print();

    cout << "\nTest 5, order 6" << endl;
    BTree<int,6>bf;
    bf.Insert(10);
    bf.Insert(20);
    bf.Insert(30);
    bf.Insert(40);
    bf.Insert(50);
    bf.Insert(60);
    bf.Insert(70);
    bf.Insert(80);
    bf.Insert(90);
    bf.Insert(100);
    bf.Insert(110);
    bf.Insert(120);
    bf.Insert(130);
    bf.Insert(140);
    bf.Insert(150);
    bf.Insert(160);
    bf.Insert(170);
    bf.Insert(180);
    bf.Insert(190);
    bf.Insert(200);
    bf.Insert(210);
    bf.Insert(220);
    bf.Insert(230);
    bf.Insert(240);
    bf.Insert(250);
    bf.Insert(260);
    bf.Insert(270);
    bf.Insert(280);
    bf.Insert(290);

    bf.Print();

    return 0;
}
