#pragma once
#include "Vector.h"

class Queue {
private:
    Vector addStack,takeStack;
    void shift()
    {
        if (addStack.size() != 0)
        {
            while (addStack.size()!=0)
                takeStack.push_back(addStack.pop_back());
        }
    }
public:
    Queue()	{};
    ~Queue() {};

    void push_back(int val)
    {
        addStack.push_back(val);
    }

    int pop_front()
    {
        if (addStack.size() == 0 && takeStack.size() == 0)
            return -1;
        else
        {
            if (takeStack.size() == 0)
                shift();
            if(takeStack.size() == 0) return -1;
            return takeStack.pop_back();
        }
    }

};