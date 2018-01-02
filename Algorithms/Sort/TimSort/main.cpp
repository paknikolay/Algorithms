#include <fstream>
#include <vector>
#include "TimSort.h"
using std:: vector;

int main() {
    std:: ifstream fin("input.txt");
    std:: ofstream fout("output.txt");
    int n;
    fin>>n;
    vector<int> m;
    for(int i = 0; i < n; ++i){
        int tmp;
        fin>>tmp;
        m.push_back(tmp);
    }
    timSort(m.begin(), m.end());

    for(int i=0; i < n; ++i){
        fout<< m[i]<<" ";
    }


    return 0;
}