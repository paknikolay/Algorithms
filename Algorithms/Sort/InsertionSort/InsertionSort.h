#pragma once

#include <vector>
#include <string>

using std::string;
using std::vector;

void insertionSort(vector<string>& arr, int size) {
    for (int i = 1; i < size; ++i) {
        string tmp = arr[i];
        int j = i - 1;
        for (; j >= 0 && tmp < arr[j]; --j)
        {
            arr[j + 1] = arr[j];
        }
        arr[j + 1] = tmp;
    }
}