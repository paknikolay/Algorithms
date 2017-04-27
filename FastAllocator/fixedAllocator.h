#pragma once
#include <stack>
using std::stack;
template <size_t chunkSize>
class FixedAllocator {
public:
    FixedAllocator() {
        size = chunkSize;
    }

    ~FixedAllocator() {
        while (!allBlocks.empty()) {
            void *ptr;
            ptr = allBlocks.top();
            allBlocks.pop();
            free(ptr);
        }
    }

    void *allocate() {
        if (freeBlocks.empty())
            fillFreeBlocks();
        void *ptr = freeBlocks.top();
        freeBlocks.pop();
        return ptr;
    }

    void deallocate(void *ptr) {
        freeBlocks.push(ptr);
    }


private:
    size_t size;
    stack<void *> freeBlocks;
    stack<void *> allBlocks;

    void fillFreeBlocks() {
        size*=2;
        void *pointIter = malloc(size);
        allBlocks.push(pointIter);
        for (int i = 0; i < size; i+=chunkSize) {
            freeBlocks.push(pointIter + i);
        }
    }
};
