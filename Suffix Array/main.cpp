// Ahmed Mohamed Noureldin ID: 20230035
// Fatema Elzhraa Ahmed ID: 20230280
// Salsabil Bahaa Eldin ID: 20230586

#include <iostream>

using namespace std;

struct Element{
  int ind; // index of suffix
  int r1; // current order of the suffix
  int r2; // order of substring starting at position [i + 2^k]

  // Overload <= operator to compare by <r1, r2>
  bool operator<=(const Element& other) const {
    if (r1 != other.r1)
      return r1 <= other.r1;
    return r2 <= other.r2;
  }
};

class SuffixArray {
private:
  char *text;
  Element *elements;
  int n;

  template<typename t>
  void merge(t arr[], int l, int mid, int r) {
    int n1 = mid - l + 1;
    int n2 = r - mid;

    t* leftArray = new t[n1];
    t* rightArray= new t[n2];

    for (int i = 0; i < n1; i++)
      leftArray[i] = arr[l+i];
    for (int i = 0; i < n2; i++)
      rightArray[i] = arr[mid+1+i];

    int i = 0, j = 0, x = l;
    while (i < n1 && j < n2) {
      if (leftArray[i] <= rightArray[j]) {
        arr[x++] = leftArray[i++];
      }
      else arr[x++] = rightArray[j++];
    }

    while (i < n1) {
      arr[x++] = leftArray[i++];
    }

    while (j < n2) {
      arr[x++] = rightArray[j++];
    }

    delete []leftArray;
    delete []rightArray;
  }

  // merge sort
  template<typename t>
  void mergeSort(t arr[], int l, int r) {
    if (l >= r) return;
    int mid = l + (r - l) / 2;
    mergeSort(arr, l, mid);
    mergeSort(arr, mid+1, r);
    merge(arr, l, mid, r);
  }

public:

  // Constructor to get the input string
  SuffixArray(const char * input){

    // Calculate the length of the input
    n = 0;
    while (input[n] != '\0')
      n++;

    // initialize dynamic arrays
    text = new char[n];
    elements = new Element[n];

    // copy the input
    int i = 0;
    while (input[i] != '\0') {
      text[i] = input[i];
      i++;
    }
    text[i] = '\0';
  }

  // Function to construct suffix array using prefix doubling
  void ConstructUsingPrefixDoubling(){
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
    for (int i = 1; i < n; i++){
      if (elements[i].r1 == elements[i-1].r1){
        classes[elements[i].ind] = classes[elements[i-1].ind];
      } else {
        classes[elements[i].ind] = classes[elements[i-1].ind] + 1;
      }
    }

    // copy classes to r1
    for (int i = 0; i < n; i++){
      elements[i].r1 = classes[i];
    }

    int k = 0;
    while ((1 << k) < n){

      for (int i = 0; i < n; i++){
        elements[i].ind = i;
        elements[i].r1 = classes[i];
        // r2 is the order of the substring starting at i + 2^k
        if (i + (1 << k) < n){
          elements[i].r2 = classes[i + (1 << k)];
        } else {
          elements[i].r2 = -1;
        }
      }

      // sort according to r1, r2
      mergeSort(elements, 0, n-1);

      classes[elements[0].ind] = 0;
      for (int i = 1; i < n; i++) {
        if (elements[i].r1 == elements[i - 1].r1 && elements[i].r2 == elements[i - 1].r2) {
          classes[elements[i].ind] = classes[elements[i - 1].ind];
        } else {
          classes[elements[i].ind] = classes[elements[i - 1].ind] + 1;
        }
      }
      k++;
    }
    // delete classes array
    delete[] classes;
  }

  // Function to print the suffix array indices
  void Print(){
    for (int i = 0; i < n; i++) {
      cout << elements[i].ind << ' ';
    }
    cout << endl;
  }

  // Destructor to deallocate the dynamic memory
  ~SuffixArray(){
    delete[] text;
    delete[] elements;
  }
};

int main() {
  SuffixArray t("ACGACTACGATAAC$");
  t.ConstructUsingPrefixDoubling();
  t.Print(); // 14 11 12  0  6  3  9 13  1  7  4  2  8 10  5

  SuffixArray t2("ababba$");
  t2.ConstructUsingPrefixDoubling();
  t2.Print(); // 6 5 0 2 4 1 3

  SuffixArray t3("aaaa$");
  t3.ConstructUsingPrefixDoubling();
  t3.Print(); // 4 3 2 1 0

  SuffixArray t4("ppppplppp$");
  t4.ConstructUsingPrefixDoubling();
  t4.Print(); // 9 5 8 4 7 3 6 2 1 0

  SuffixArray t5("bananabana$");
  t5.ConstructUsingPrefixDoubling();
  t5.Print(); // 10 9 5 7 3 1 6 0 8 4 2

  SuffixArray t6("abtababtabt$");
  t6.ConstructUsingPrefixDoubling();
  t6.Print(); // 11 3 8 0 5 4 9 1 6 10 2 7
  return 0;
}
