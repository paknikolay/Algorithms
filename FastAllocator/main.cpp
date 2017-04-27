#include <iostream>
#include <set>
#include <chrono>
#include <list>
#include "fastAllocator.h"
using std::list;
using std::cout;


void test() {
    list<int, std::allocator<int>> list1;
    list<int, FastAllocator<int>> list2;

    std::chrono::time_point<std::chrono::system_clock> s, f;
    s = std::chrono::system_clock::now();
srand(0);
    int n = 100000;
    for (int i = 0; i < n; ++i) {
        if ((rand() * i + rand()) % 2 == 0)
            list1.push_back(i);
        else if (list1.size() == 0) list1.push_back(i);
        else list1.pop_back();
    }

    f = std::chrono::system_clock::now();
    long long t1 = std::chrono::duration_cast<std::chrono::nanoseconds >
            (f-s).count();
cout<<"std::allocator "<<t1<<"\n";
////////////////////////////////////
    srand(0);
    s = std::chrono::system_clock::now();
    for (int i = 0; i < n; ++i) {
        if ((rand() * i + rand()) % 2 == 0)
            list2.push_back(i);
        else if (list2.size() == 0) list2.push_back(i);
        else list2.pop_back();
    }
    f = std::chrono::system_clock::now();
   long long t = std::chrono::duration_cast<std::chrono::nanoseconds>
            (f-s).count();
    cout<<"FastAllocator "<<t<<"\n";
cout<<t*1.0/t1;
}

struct ForTest{
int a;
char b;
long long c;
};

void test2(){
    int n = 1000;
    stack <std::pair<ForTest*, int>> v;
    FastAllocator<ForTest> allocator;
    for (int i = 0; i < n; ++i) {
        int size;
        ForTest* forTest = allocator.allocate(size = rand()%(i+5));
        v.push(std::make_pair(forTest, size));
        if(i%(((rand()+1)*rand() +1))%(i + 5)){
            allocator.deallocate(v.top().first, v.top().second);
            v.pop();
        }
        //allocator.deallocate(forTest, size);
    }
    while(!v.empty()){
        allocator.deallocate(v.top().first, v.top().second);
        v.pop();

    }
}


int main() {
test2();
    return 0;
}