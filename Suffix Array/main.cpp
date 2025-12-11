// Ahmed Mohamed Noureldin ID: 20230035
// Fatema Elzhraa Ahmed ID: 20230280
// Salsabil Bahaa Eldin ID: 20230586

#include <iostream>

using namespace std;

struct Element
{
    int ind; // index of suffix
    int r1; // current order of the suffix
    int r2; // order of substring starting at position [i + 2^k]

    // Overload <= operator to compare by <r1, r2>
    bool operator<=(const Element& other) const
    {

        // comparing between r1 and r1 of the other
        if (r1 != other.r1)
            return r1 <= other.r1;
        // in case of equality compare between r2 and r2 of other
        return r2 <= other.r2;
    }
};

class SuffixArray
{
private:
    char *text;// input string
    Element *elements;// suffix substrings of input string
    int n;// Length of the word

    // merging function
    template<typename t>
    void merge(t arr[], int l, int mid, int r)
    {
        // sizes of the two sub-arrays
        int n1 = mid - l + 1;
        int n2 = r - mid;

        //Allocating two sub-arrays
        t* leftArray = new t[n1];
        t* rightArray= new t[n2];

        //putting values in the two sub-arrays
        for (int i = 0; i < n1; i++)
            leftArray[i] = arr[l+i];
        for (int i = 0; i < n2; i++)
            rightArray[i] = arr[mid+1+i];

        //Iterators
        int i = 0;int j = 0;int x = l;

        // looping and merging the two sub-arrays in the correct place
        while (i < n1 && j < n2)
        {
            if (leftArray[i] <= rightArray[j])
            {
                arr[x++] = leftArray[i++];
            }
            else arr[x++] = rightArray[j++];
        }

        // Checking is there any left elements in left Array
        while (i < n1)
        {
            arr[x++] = leftArray[i++];
        }

        // Checking is there any left elements in left Array
        while (j < n2)
        {
            arr[x++] = rightArray[j++];
        }

        // deallocating the used space
        delete []leftArray;
        delete []rightArray;
    }

    // merge sort
    template<typename t>
    void mergeSort(t arr[], int l, int r)
    {
        // base case
        if (l >= r) return;
        // splitting sub-array into two sub-arrays
        int mid = l + (r - l) / 2;
        //left side
        mergeSort(arr, l, mid);
        //right side
        mergeSort(arr, mid+1, r);
        //merging the two sub-arrays
        merge(arr, l, mid, r);
    }

public:

    // Constructor to get the input string
    SuffixArray(const char * input)
    {

        // Calculate the length of the input
        n = 0;
        // counting the length of the input and incrementing n
        while (input[n] != '\0')
            n++;

        // initialize dynamic arrays
        text = new char[n];
        elements = new Element[n];

        // copy the input
        int i = 0;
        while (input[i] != '\0')
        {
            text[i] = input[i];
            i++;
        }
        text[i] = '\0';
    }

    // Function to construct suffix array using prefix doubling
    void ConstructUsingPrefixDoubling()
    {
        // at k = 0
        for (int i = 0; i < n; i++){
            elements[i].ind = i;
            elements[i].r1 = text[i];
            elements[i].r2 = 0;
        }

        // sort according to r1, r2
        mergeSort(elements, 0, n-1);

        int *classes = new int[n];
        classes[elements[0].ind] = 0;
        for (int i = 1; i < n; i++)
        {
            if (elements[i].r1 == elements[i-1].r1)
            {
                classes[elements[i].ind] = classes[elements[i-1].ind];
            } else
            {
                classes[elements[i].ind] = classes[elements[i-1].ind] + 1;
            }
        }

        // copy classes to r1
        for (int i = 0; i < n; i++)
        {
            elements[i].r1 = classes[i];
        }

        int k = 0;
        while ((1 << k) < n)
        {

            for (int i = 0; i < n; i++)
            {
                elements[i].ind = i;
                elements[i].r1 = classes[i];
                // r2 is the order of the substring starting at i + 2^k
                if (i + (1 << k) < n)
                {
                    elements[i].r2 = classes[i + (1 << k)];
                } else
                {
                    elements[i].r2 = -1;
                }
            }

            // sort according to r1, r2
            mergeSort(elements, 0, n-1);

            classes[elements[0].ind] = 0;
            for (int i = 1; i < n; i++)
            {
                if (elements[i].r1 == elements[i - 1].r1 && elements[i].r2 == elements[i - 1].r2)
                {
                    classes[elements[i].ind] = classes[elements[i - 1].ind];
                } else
                {
                    classes[elements[i].ind] = classes[elements[i - 1].ind] + 1;
                }
            }
            k++;
        }
        // delete classes array
        delete[] classes;
    }

    // Function to print the suffix array indices
    void Print()
    {
        // iterating on each element and printing it
        for (int i = 0; i < n; i++)
        {
            cout << elements[i].ind << ' ';
        }
        cout << endl;
    }

    // Destructor to deallocate the dynamic memory
    ~SuffixArray()
    {
        //deallocating spaces
        delete[] text;
        delete[] elements;
    }
};


