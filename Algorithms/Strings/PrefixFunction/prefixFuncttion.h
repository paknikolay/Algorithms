#pragma once
#include <iostream>
#include <vector>
#include <string>

using std::cin;
using std::cout;
using std::vector;
using std::string;

void prefixFinction(string& s, vector<int>& prefixFunction) {
    p[0] = 0;
    for (int i = 1; i <= s.size(); ++i) {
        int k = p[i - 1];
        while (k > 0 && s[k] != s[i]) {
            k = p[k-1];
        }
        p[i] = (s[k] == s[i]) ? k + 1 : 0;
    }
}