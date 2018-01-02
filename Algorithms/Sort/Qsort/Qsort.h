#pragma once
#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;
#include<vector>

void Qsort(int* v, int size)
{
    if (size < 30)
    {
        for (int i = 1; i < size; ++i) {
            int tmp = v[i];
            int j = i - 1;
            for (; j >= 0 && tmp < v[j]; --j)
            {
                v[j + 1] = v[j];
            }
            v[j + 1] = tmp;
        }

        return;
    }
    size -= 1;
    int piv = (rand() % size);
    swap(v[piv], v[size]);
    int first = 0, last;
    while (first < size  && v[first] <= v[size])
        ++first;

    last = first + 1;
    while (last < size)
    {
        if (v[last] <= v[size]) {
            swap(v[last], v[first]);
            ++first;
        }
        ++last;
    }
    swap(v[first], v[size]);
    Qsort(v, first);
    Qsort(v + first + 1, size - first);
}
