#pragma once
#include <stack>
#include <cmath>
#include <vector>
using std::vector;
using std::stack;
using std::iter_swap;
using std::pair;
typedef unsigned int ui32;

#include "Run.h"
#include "Sorts.h"
#include "ITimSortParams.h"
template <class RandomAccessIterator, class Compare>
void timSort(RandomAccessIterator first, RandomAccessIterator last, Compare comp, const ITimSortParams&  params = TimSortParams());

template <class RandomAccessIterator>
void timSort(RandomAccessIterator first, RandomAccessIterator last, const ITimSortParams&  params = TimSortParams());

