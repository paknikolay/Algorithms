#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include "Network.h"

using std::cin;
using std::cout;
using std::vector;
using std::queue;

int edmonds_karp(vector<vector<int>>& v, int n, int m, int sink, int source){
    int flow = 0;
    while(true) {
        int *vs = new int[n];
        int *vback = new int [n];
        for (int j = 0; j < n; ++j) {
            vs[j] = -1;
            vback[j] = -1;
        }

        queue<int> q;
        q.push(source);
        int ind = 0;
        vs[source] = 0;
        int currentV;
        while(!q.empty()) {
            currentV = q.front();//mb back
            q.pop();
            for (int i = 0; i < n; ++i) {
                if (v[currentV][i] > 0 && vs[i] == -1){
                    q.push(i);
                    vs[i] = vs[currentV] + 1;
                    vback[i] = currentV;
                }
            }
            if(vback[sink] != -1){
                ind = 1;
                break;
            }
        }

        if(ind == 0) break; // no way to the sink
        //else
        int minAmount  = 1000000000; // max amount
        currentV = sink;
        while(vback[currentV] != -1){
            int prevV = vback[currentV];
            if(v[prevV][currentV] < minAmount)
                minAmount = v[prevV][currentV];
            currentV = prevV;
        }

        currentV = sink;
        while(vback[currentV] != -1){
            int prevV = vback[currentV];
            v[prevV][currentV] -= minAmount;
            v[currentV][prevV]+= minAmount;
            currentV = prevV;
        }
        flow += minAmount;
        delete[] vs;
        delete[] vback;
    }
    return flow;
}
