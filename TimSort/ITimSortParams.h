#pragma once
#ifndef TIMSORT_ITIMSORTPARAMS_H
#define TIMSORT_ITIMSORTPARAMS_H

enum class EWhatMerge {
    WM_NoMerge,
    WM_MergeXY,
    WM_MergeYZ
};


class ITimSortParams {
public:
    // Вычисление minRun по n.
    virtual ui32 minRun(ui32 size) const = 0;

    // Нужно ли выполнять слияние, если в стеке ровно два рана размеров lenX и lenY.
    virtual bool needMerge(ui32 lenX, ui32 lenY) const = 0;

    // Как сливать, если в стеке три последних рана имеют размеры lenX, lenY и lenZ.
    virtual EWhatMerge whatMerge(ui32 lenX, ui32 lenY, ui32 lenZ) const = 0;
};

class TimSortParams : public ITimSortParams {
public:
    // Вычисление minRun по n.
    virtual ui32 minRun(ui32 size) const;

    // Нужно ли выполнять слияние, если в стеке ровно два рана размеров lenX и lenY.
    virtual bool needMerge(ui32 lenX, ui32 lenY) const;

    // Как сливать, если в стеке три последних рана имеют размеры lenX, lenY и lenZ.
    virtual EWhatMerge whatMerge(ui32 lenX, ui32 lenY, ui32 lenZ) const;
};
#endif //TIMSORT_ITIMSORTPARAMS_H
