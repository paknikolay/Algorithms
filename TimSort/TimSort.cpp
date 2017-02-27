#include "TimSort.h"
template <class RandomAccessIterator, class Compare>
void timSort(RandomAccessIterator first, RandomAccessIterator last, Compare comp, const ITimSortParams&  params = TimSortParams()) {
    if (last - first <= MINUMUM_FOR_QUADRATIC_SORT) {
        selectionSort(first, last, comp);
        return;
    }

    ui32 minRun = params.minRun(last - first);
    vector<pair<RandomAccessIterator, ui32 >> runs;
    RandomAccessIterator i = first;
    pair<RandomAccessIterator, ui32> tmp;
    while (i < last) {
        tmp.second = makeRuns(i, last, comp, minRun);
        tmp.first = i;
        runs.push_back(tmp);
        i += tmp.second;
        if (runs.size() >= 3) {

            EWhatMerge whatMerge = params.whatMerge(runs[runs.size() - 1].second, runs[runs.size() - 2].second,
                                                    runs[runs.size() - 3].second);
            while ((runs.size() >= 3) && (whatMerge != EWhatMerge::WM_NoMerge)) {
                if (whatMerge == EWhatMerge::WM_MergeYZ) {
                    merge(runs[runs.size() - 3].first, runs[runs.size() - 3].second + runs[runs.size() - 2].second,
                          comp);
                    runs[runs.size() - 3].second += runs[runs.size() - 2].second;
                    runs[runs.size() - 2] = runs[runs.size() - 1];
                    runs.pop_back();
                } else {
                    merge(runs[runs.size() - 2].first, runs[runs.size() - 2].second + runs[runs.size() - 1].second,
                          comp);
                    runs[runs.size() - 2].second += runs[runs.size() - 1].second;
                    runs.pop_back();
                }
                if (runs.size() >= 3)
                    whatMerge = params.whatMerge(runs[runs.size() - 1].second, runs[runs.size() - 2].second,
                                                 runs[runs.size() - 3].second);
            }
        }
        if ((runs.size() == 2) && params.needMerge(runs[runs.size() - 1].second, runs[runs.size() - 2].second)) {
            merge(runs[runs.size() - 2].first, runs[runs.size() - 1].second + runs[runs.size() - 2].second, comp);
            runs[runs.size() - 2].first = runs[runs.size() - 2].first;
            runs[runs.size() - 2].second = runs[runs.size() - 1].second + runs[runs.size() - 2].second;
            runs.pop_back();
        }

    }
    while (runs.size() >= 3) {
        merge(runs[runs.size() - 2].first, runs[runs.size() - 2].second + runs[runs.size() - 1].second, comp);
        runs[runs.size() - 2].second += runs[runs.size() - 1].second;
        runs.pop_back();
    }

    if (runs.size() == 2) {
        merge(runs[runs.size() - 2].first, runs[runs.size() - 1].second + runs[runs.size() - 2].second, comp);
        runs[runs.size() - 2].first = runs[runs.size() - 2].first;
        runs[runs.size() - 2].second = runs[runs.size() - 1].second + runs[runs.size() - 2].second;
        runs.pop_back();
    }
}

template <class RandomAccessIterator>
void timSort(RandomAccessIterator first, RandomAccessIterator last, const ITimSortParams&  params = TimSortParams()) {
    timSort(first, last, std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>(), params);
}


