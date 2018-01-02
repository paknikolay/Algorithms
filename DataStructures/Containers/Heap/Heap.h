#pragma once

#include <vector>

using std::vector;
using std::swap;
//buiding heap from array O(n)
template <typename T>
class Heap {
public:
    vector<T> v;
    int size;

    Heap() {};

    ~Heap() {};

    void siftDown(long i) {
        if (size > 0) {
            int i1 = 2 * i + 1, i2 = 2 * i + 2;
            if (i2 < size) {
                if (v[i] < v[i1] || v[i] < v[i2])
                    if (v[i1] >= v[i2]) {
                        swap(v[i], v[i1]);
                        siftDown(i1);
                    } else {
                        swap(v[i], v[i2]);
                        siftDown(i2);
                    }
            } else if (i1 < size)
                if (v[i1] > v[i])
                    swap(v[i], v[i1]);
        }
    }


    void buildHeap() {
        size = v.size();
        for (long i = v.size() - 1; i >= 0; --i)
            siftDown(i);
    }
};
