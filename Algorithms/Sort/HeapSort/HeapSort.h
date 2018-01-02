#include "Heap.h"
#include <iostream>
#include <vector>

template <typename T>
void heapSort(vector<T>& vectr) {
    Heap<T> heap;
    for (long i = 0; i < vectr.size(); ++i) {
        heap.v.push_back(vectr[i]);
    }

    heap.buildHeap();
    long kol = heap.v.size();
    for (long i = 0; i < kol - 1; i++) {
        swap(heap.v[0], heap.v[heap.size - 1]);
        --heap.size;
        heap.siftDown(0);
    }
    for (int i = 0; i < vectr.size(); ++i) {
        vectr[i] = heap.v[i];
    }
}
