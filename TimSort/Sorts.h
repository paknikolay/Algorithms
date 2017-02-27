
#ifndef TIMSORT_SORTS_H
#define TIMSORT_SORTS_H



enum {
    MINUMUM_FOR_QUADRATIC_SORT = 16,
    GALLOP = 7
};
template <class RandomAccessIterator, class Compare>
void insertionSort(RandomAccessIterator first, RandomAccessIterator last, Compare comp, RandomAccessIterator start);

template <class RandomAccessIterator, class Compare>
void insertionSort(RandomAccessIterator first, RandomAccessIterator last, Compare comp);

template <class RandomAccessIterator, class Compare>
void selectionSort(RandomAccessIterator first, RandomAccessIterator last, Compare comp);

template <class RandomAccessIterator>
void swapGroup(RandomAccessIterator firstBegin, RandomAccessIterator secondBegin, ui32 size);

template <class RandomAccessIterator, class Compare>
ui32 findAmountForMerging(RandomAccessIterator begin, RandomAccessIterator end, RandomAccessIterator other, Compare comp);


template <class RandomAccessIterator, class Compare>
ui32 gallopMerge(RandomAccessIterator begin, RandomAccessIterator end, RandomAccessIterator other, RandomAccessIterator i, Compare comp);


template <class RandomAccessIterator, class Compare>
void mergeGroup(RandomAccessIterator first, RandomAccessIterator second, RandomAccessIterator buffer, ui32 size, Compare comp);

template <class RandomAccessIterator, class Compare>
void merge(RandomAccessIterator begin, ui32 size, Compare comp);
#endif //TIMSORT_SORTS_H
