//
// Created by User on 027 27.02.17.
//

#include "ITimSortParams.h"

ui32 TimSortParams::minRun(ui32 size) const {
    ui32 r = 0;
    while (size >= 64) {
        r |= size & 1;
        size >>= 1;
    }
    return size + r;
}

// Нужно ли выполнять слияние, если в стеке ровно два рана размеров lenX и lenY.
bool TimSortParams::needMerge(ui32 lenX, ui32 lenY) const {
    return lenX >= lenY;
}

// Как сливать, если в стеке три последних рана имеют размеры lenX, lenY и lenZ.
EWhatMerge TimSortParams::whatMerge(ui32 lenX, ui32 lenY, ui32 lenZ) const {
    EWhatMerge wMerge;
    if ((lenZ <= lenX + lenY) || (lenY <= lenX)) {
        if (lenZ <= lenX)
            return EWhatMerge::WM_MergeYZ;
        else
            return EWhatMerge::WM_MergeXY;
    } else
        return EWhatMerge::WM_NoMerge;
}
