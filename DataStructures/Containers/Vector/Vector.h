#pragma once
#include <iostream>

class Vector {
private:
    long bufferSize, currentSize;
    int *buffer;

    void grow() {
        bufferSize *= 2;
        int *tmp = buffer;
        buffer = new int[bufferSize];
        for (int i = 0; i < bufferSize; i++) {
            buffer[i] = tmp[i];
        }
        delete[] tmp;
    }

    void decrease() {
        bufferSize /= 4;
        int *tmp = buffer;
        buffer = new int[bufferSize];
        for (int i = 0; i < bufferSize; i++)
            buffer[i] = tmp[i];
        delete[]tmp;
    }

public:
    explicit Vector() {
        bufferSize = 16;
        currentSize = 0;
        buffer = new int[bufferSize];
    };

    ~Vector() {
        delete[] buffer;
    };

    void push_back(int val) {
        if (currentSize + 1 <= bufferSize) {
            currentSize += 1;
            buffer[currentSize - 1] = val;
        } else {
            grow();
            currentSize += 1;
            buffer[currentSize - 1] = val;
        }
    }

    int pop_back() {
        if (currentSize >= 1) {
            int tmp_val = buffer[currentSize - 1];
            currentSize -= 1;
            //if ((currentSize == bufferSize / 4) && (currentSize>16))decrease();
            return tmp_val;
        } else return -1;
    }

    long size() {
        return currentSize;
    }

};

