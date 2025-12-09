#include <iostream>

using namespace std;

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


int main() {

    return 0;
}