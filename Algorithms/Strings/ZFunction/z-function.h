#pragma once
#include <iostream>
#include <vector>
#include <string>

using std::string;
using std:: vector;
using std:: cout;
using std:: cin;

void z_function(const string& s, vector<int>& z) {
    int left = 0, right = 0;
    for (int i = 1; i < z.size(); ++i) {
        if (i <= right) {
            z[i] = std::min(right - i + 1, z[i - left]);
        }

        while (i + z[i] < s.size() && s[z[i]] == s[i + z[i]])
            ++z[i];
        if (i + z[i] - 1 > right) {
            left = i;
            right = i + z[i] - 1;
        }
    }
    z[0] = z.size();
}