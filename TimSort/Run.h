#pragma once
#ifndef TIMSORT_RUN_H
#define TIMSORT_RUN_H


template <class RandomAccessIterator>
void reverseRun(RandomAccessIterator first, RandomAccessIterator last);
template <class RandomAccessIterator, class Compare>
ui32 makeRuns(RandomAccessIterator first, RandomAccessIterator last, Compare comp, ui32 minRun);

#endif //TIMSORT_RUN_H
