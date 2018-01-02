#pragma once

#include <iostream>
#include <string>
using namespace std;
#include <vector>

void countingSort(string** v, int n, int* groups, int num, int k, int p)
{
    for (int i = 0; i < k + 2; ++i)
        groups[i] = 0;

    for (int i = 0; i < n; ++i)
        ++groups[(((*(v[i]))[num] - 'a') + 1 + p) % p];

    int sum = 0;
    for (int i = 0; i < k + 2; ++i)
    {
        int tmp = groups[i];
        groups[i] = sum;
        sum += tmp;
    }
    string **tm;
    tm = new string*[n];

    for (int i = 0; i < n; ++i)
        tm[groups[(((*(v[i]))[num] - 'a') + 1 + p) % p]++] = v[i];

    for (int i = 0; i < n; ++i)
        v[i] = tm[i];

    delete[] tm;
}
void MSDSort(string**v, int n, int num, int  k, int p)
{
    if (n > 1) {
        int *groups = new int[k + 2];
        countingSort(v, n, groups, num, k, p);
        for (int i = 1; i < k; ++i)
            MSDSort(v + groups[i - 1], groups[i] - groups[i - 1], num + 1, k, p);
        delete[] groups;
    }
}