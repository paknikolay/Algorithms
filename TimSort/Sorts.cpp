
#include "Sorts.h"

template <class RandomAccessIterator, class Compare>
void insertionSort(RandomAccessIterator first, RandomAccessIterator last, Compare comp, RandomAccessIterator start) {
    for (RandomAccessIterator i = start; i < last; ++i) {
        typename std::iterator_traits<RandomAccessIterator>::value_type tmp = *i;
        RandomAccessIterator j = i - 1;
        for (; j >= first && comp(tmp, *j); --j) {
            iter_swap(j, j + 1);
        }
        *(j + 1) = tmp;
    }
}
template <class RandomAccessIterator, class Compare>
void insertionSort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    insertionSort(first, last, comp, first);
}
template <class RandomAccessIterator, class Compare>
void selectionSort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    for (RandomAccessIterator i = first; i < last; ++i) {
        RandomAccessIterator minim = i;
        for (RandomAccessIterator j = i + 1; j < last; ++j) {
            if (comp(*j, *minim))
                minim = j;
        }
        iter_swap(i, minim);
    }
}

template <class RandomAccessIterator>
void swapGroup(RandomAccessIterator firstBegin, RandomAccessIterator secondBegin, ui32 size) {
    for (ui32 i = 0; i < size; ++i) {
        iter_swap(firstBegin + i, secondBegin + i);
    }
}

template <class RandomAccessIterator, class Compare>
ui32 findAmountForMerging(RandomAccessIterator begin, RandomAccessIterator end, RandomAccessIterator other, Compare comp) {
    ui32 amount = 0;
    ui32 size = end - begin;
    ui32 first = 1;
    ui32 last = 1;
    while ((begin + last < end) && (comp(*(begin + std::min(last, size)), *other))) {
        first = last;
        last = std:: min(size, last * 2);
    }

    while (first < last - 1) {
        ui32 mid = (first + last) / 2;
        if (comp(*(begin + mid), *other))
            first = mid;
        else
            last = mid;
    }
    return  first-1;
};

template <class RandomAccessIterator, class Compare>
ui32 gallopMerge(RandomAccessIterator begin, RandomAccessIterator end, RandomAccessIterator other, RandomAccessIterator i, Compare comp) {
    if (end == begin)
        return 0;
    ui32 size = findAmountForMerging(begin, end, other, comp) ;
    for (ui32 j = 0; j < size; ++j) {
        iter_swap(i + j, begin + j);
    }
    return size;
};


template <class RandomAccessIterator, class Compare>
void mergeGroup(RandomAccessIterator first, RandomAccessIterator second, RandomAccessIterator buffer, ui32 size, Compare comp) {
    swapGroup(first, buffer, size);
    RandomAccessIterator bufferTake = buffer;
    RandomAccessIterator secondTake = second;
    RandomAccessIterator put = first;
    ui32 fCount = 0, sCount = 0;
    for (ui32 i = 0; i < 2 * size; ++i) {
        bool a = secondTake == second + size,
                b= (bufferTake < buffer + size) && comp(*bufferTake, *secondTake);
        if ((a) || (b)){
            iter_swap(put, bufferTake);
            ++put;
            ++bufferTake;
            ++fCount;
            sCount = 0;
            if (fCount == GALLOP && bufferTake < buffer + size && secondTake < second + size) {
                ui32 s = gallopMerge(bufferTake, buffer + size, secondTake, put, comp);
                bufferTake += s;
                put += s;
                i += s;
            }
        }
        else {
            iter_swap(put, secondTake);
            ++put;
            ++secondTake;
            ++sCount;
            fCount = 0;
            if (sCount == GALLOP && secondTake < second + size && bufferTake < buffer + size) {
                ui32 s = gallopMerge(secondTake, second + size, bufferTake, put, comp);
                secondTake += s;
                put += s;
                i += s;
            }
        }
    }
}

template <class RandomAccessIterator, class Compare>
void merge(RandomAccessIterator begin, ui32 size, Compare comp) {
    if (size <= MINUMUM_FOR_QUADRATIC_SORT) {
        selectionSort(begin, begin + size, comp);
        return;
    }
    ui32 groupSize = sqrt(size);
    ui32 restSize = size % groupSize;
    ui32 amountOfGroups = size / groupSize - 1;

    RandomAccessIterator border = begin;
    while ((border < begin + groupSize * amountOfGroups) &&
           (!comp(*(border + 1), *border)))
        ++border;

    border = border - ((border - begin) % groupSize);
    swapGroup(border, begin + amountOfGroups * groupSize, groupSize);
    restSize += groupSize;


    for (RandomAccessIterator i = begin; i < begin + groupSize * amountOfGroups; i += groupSize) {
        RandomAccessIterator minim = i;
        for (RandomAccessIterator j = i + groupSize; j < begin + groupSize * amountOfGroups; j += groupSize)
            if (comp(*j, *minim))
                minim = j;
            else {
                bool a = comp(*minim, *j);
                if (!(comp(*minim, *j)) && comp(*(j + groupSize - 1), *(minim + groupSize - 1)))
                    minim = j;
            }
        swapGroup(i, minim, groupSize);
    }


    for (RandomAccessIterator i = begin; i < begin + groupSize * (amountOfGroups - 1); i += groupSize) {

        mergeGroup(i, i + groupSize, begin + amountOfGroups * groupSize, groupSize, comp);
    }

    selectionSort(begin + size - 2 * restSize, begin + size, comp);

    for (RandomAccessIterator i = begin + size - 2 * restSize; i >= begin + restSize; i -= restSize)
        mergeGroup(i - restSize, i, begin + size - restSize, restSize, comp);

    selectionSort(begin, begin + 2 * restSize, comp);
    selectionSort(begin + size - 2 * restSize, begin + size, comp);
}


