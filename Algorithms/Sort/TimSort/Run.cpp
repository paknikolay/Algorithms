//
// Created by User on 027 27.02.17.
//

#include "Run.h"


template <class RandomAccessIterator>
void reverseRun(RandomAccessIterator first, RandomAccessIterator last) {
    for (RandomAccessIterator i = first; i< first + (last - first) / 2; ++i) {
        iter_swap(i, last - (i - first) - 1);
    }
}
template <class RandomAccessIterator, class Compare>
ui32 makeRuns(RandomAccessIterator first, RandomAccessIterator last, Compare comp, ui32 minRun) {
    ui32 size = last - first;
    if (size == 1)
        return 1;
    ui32 runSize = 2;
    bool isIncreasing = comp(*first, *(first + 1)) ? true : false;
    RandomAccessIterator i = first + 2;
    while ((i < last) && (isIncreasing ? !(comp(*i, *(i - 1))) : !(comp(*(i - 1), *i)))) {
        ++runSize;
        ++i;
    }

    if (!isIncreasing)
        reverseRun(first, first + runSize);

    if (runSize < minRun) {
        insertionSort(first, first + std::min(minRun, size), comp, first + runSize);
        runSize = std::min(minRun, size);
    }
    return runSize;
};
