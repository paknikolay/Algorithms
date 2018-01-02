#pragma once

#include "geometricPrimitives.h"
#include <list>
#include <vector>

using std::vector;
using std::list;
//count - how to mark dots in convex hall
void jarvis(list<Dot>& dots, int count, vector<int>&numbers){
    long double e = 0.00000001;
    typename  list<Dot>::iterator i = dots.begin(), mi = i;
    for (; i != dots.end();++i) {
        if(i->y < mi->y || (i->y == mi->y && i->x > mi->x))
            mi = i;
    }
    Dot m = *mi;
    numbers[mi->number] = count;
    dots.erase(mi);
    auto p0 =  dots.begin();
    i = dots.begin();
    ++i;
    for (;i!= dots.end(); ++ i) {
        if(isLess(*i, *p0, m)) p0 = i;
    }
    numbers[p0->number] = count;
    Dot p = *p0;
    dots.erase(p0);
    dots.push_back(m);
    while (true) {
        p0 =  dots.begin();
        i = dots.begin();
        ++i;
        for (;i!= dots.end(); ++ i) {
            if(isLess(*i, *p0, p)) p0 = i;
        }
        numbers[p0->number] = count;
        p = *p0;
        dots.erase(p0);
        if(m.number == p.number){
            break;
        }

    }

}