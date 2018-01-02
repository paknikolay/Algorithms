#pragma once
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
using std::vector;
using std::cout;
using std::cin;
using std::max;
using std::min;
using std::pair;
using std::make_pair;


class Treap {// left < right
public:
    Treap() {
        size = 0;
        root = nullptr;
        srand(time(0));
    }

    ~Treap() {
        destroyTree(root);
    }


    void prevPermutation(size_t first, size_t last) {
        makePermutation(first, last, true);
    }

    void nextPermutation(size_t first, size_t last) {
        makePermutation(first, last, false);
    }

    long long getSum(size_t first, size_t last) {
        return getSum(first, last, root);
    }

    void reverse(size_t first, size_t last) {
        makeAction(first, last, Action::reverseAction);
    }

    void assign(size_t first, size_t last, long long value) {
        makeAction(first, last, Action::assignAction, value);
    }

    void add(size_t first, size_t last, long long value) {
        makeAction(first, last, Action::addAction, value);
    }

    void insert(long long value) {
        insert(root, size, rand(), value);
        ++size;
    }

    void insert(long long value, size_t index) {
        insert(root, index, rand(), value);
        ++size;
    }

    void remove(size_t index) {
        remove(root, index);
    }

    void print() {
        print(root);
        cout << "\n";
    }

    long long getSize() {
        return size;
    }

private:
    enum class Monotony {
        increase, decrease, permanent, noMonotony
    };

    struct Node {
        Node(long long _priority, long long _value) {
            parent = left = right = nullptr;
            priority = _priority;
            amount = 1;
            value = _value;
            isReversed = false;
            isAdded = false;
            isAssigned = false;
            added = 0;
            assigned = 0;
            sum = _value;
            isRising = isDecreasing = true;
            max = min = _value;
        }

        Node *left, *right;
        Node *parent;
        long long sum;
        size_t index;
        long long priority;
        long long value;
        long long amount;
        long long added;
        long long assigned;
        bool isReversed;
        bool isAdded;
        bool isAssigned;
        bool isRising;  // == is possible
        bool isDecreasing;
        long long max;
        long long min;

    };

    Node *root;
    long long size;

    enum class Action {
        reverseAction, addAction, assignAction
    };

    void makeAction(size_t first, size_t last, Action action, long long value = 0) {
        Node *right, *left, *middle;
        split(root, middle, right, last + 1);
        split(middle, left, middle, first);
        switch (action) {
            case Action::reverseAction:
                middle->isReversed = true;
                break;
            case Action::addAction:
                middle->isAdded = true;
                middle->added = value;
                break;
            case Action::assignAction:
                middle->isAssigned = true;
                middle->assigned = value;
                break;
        }
        root = merge(merge(left, middle), right);
        fixNode(root);
    }

    size_t findBeginningOfSuffix(Node *current, size_t index, bool perm) { //true = prev false = next
        updateNode(current);
        if (current == nullptr)
            return index;
        updateNode(current->left);
        updateNode(current->right);
        if (perm ? current->isDecreasing : current->isRising)
            return index;
        else {
            pair<Monotony, Monotony> monotony = indentifyMonotony(current);// left, right
            if (monotony.second == (perm ? Monotony::decrease : Monotony::increase) ||
                monotony.second == Monotony::permanent) {
                if (current->left == nullptr)
                    return index;
                Node *right = findSideValue(current->left, false);
                if (perm ? right->value <= current->value : right->value >= current->value)
                    return findBeginningOfSuffix(current->left, index, perm);
                else
                    return index + getAmount(current->left);
            } else {
                if (current->right == nullptr)
                    return index + getAmount(current->left);
                else
                    return findBeginningOfSuffix(current->right, index + getAmount(current->left) + 1, perm);
            }
        }
    }

    void makePermutation(size_t first, size_t last, bool perm) { // true = prev false = next
        Node *right, *left, *middle;
        split(root, middle, right, last + 1);
        split(middle, left, middle, first);
        if (perm ? middle->isDecreasing : middle->isRising)
            middle->isReversed ^= 1;
        else {
            size_t index = findBeginningOfSuffix(middle, 0, perm);
            Node *rest, *suffix;
            split(middle, rest, suffix, index);
            Node *nodeForSwapFromTheRest;
            split(rest, rest, nodeForSwapFromTheRest, rest->amount - 1);

            size_t indexOfSwappingNode = findNodeForPerm(suffix, nodeForSwapFromTheRest->value, 0, perm);

            Node *leftSuffix, *middleSuffix, *rightSuffix;
            split(suffix, middleSuffix, rightSuffix, indexOfSwappingNode + 1);
            split(middleSuffix, leftSuffix, middleSuffix, indexOfSwappingNode);

            std::swap(middleSuffix, nodeForSwapFromTheRest);

            fixNode(middleSuffix);
            fixNode(nodeForSwapFromTheRest);
            fixNode(middleSuffix);
            suffix = merge(merge(leftSuffix, middleSuffix), rightSuffix);

            suffix->isReversed ^= 1;
            rest = merge(rest, nodeForSwapFromTheRest);
            middle = merge(rest, suffix);
        }
        root = merge(merge(left, middle), right);
        fixNode(root);
    }


    Monotony getMonotony(Node *current) {//1 rising -1 decreasing 0 constant -2 nothing
        updateNode(current);
        if (current == nullptr)
            return Monotony::permanent;
        if (current->isDecreasing && current->isRising)
            return Monotony::permanent;
        else if (current->isRising)
            return Monotony::increase;
        else if (current->isDecreasing)
            return Monotony::decrease;
        else
            return Monotony::noMonotony;
    }

    long long getSum(long first, long last, Node *current) {
        if (current == nullptr || first > last)
            return 0;
        fixNode(current);
        size_t l = getAmount(current->left);
        if (first == 0 && last + 1 == current->amount)
            return getSum(current);
        if (l >= first && l <= last) {
            return current->value + getSum(first, l - 1, current->left) + getSum(0, last - l - 1, current->right);
        } else if (l < first) {
            return getSum(first - l - 1, last - l - 1, current->right);
        } else {
            return getSum(first, last, current->left);
        }


    }

    void destroyTree(Node *current) {
        if (current == nullptr)
            return;
        destroyTree(current->right);
        destroyTree(current->left);
        delete current;
    }

    long long getAmount(Node *current) {
        if (current == nullptr)
            return 0;
        else
            return current->amount;
    }

    long long getSum(Node *current) {
        if (current == nullptr)
            return 0;
        updateNode(current);
        return current->sum;
    }

    long long getValue(Node *current) {
        if (current == nullptr)
            return 0;
        updateNode(current);
        return current->value;
    }

    void inline updateSide(Node *current, bool isLeft) {
        if (isLeft ? current->left != nullptr : current->right != nullptr) {
            if (current->isReversed) {
                isLeft ? current->left->isReversed ^= 1 : current->right->isReversed ^= 1;
            }
            if (current->isAdded) {
                if (isLeft ? current->left->isAdded : current->right->isAdded) {
                    isLeft ? current->left->added += current->added : current->right->added += current->added;
                } else {
                    if (isLeft ? current->left->isAssigned : current->right->isAssigned) {
                        isLeft ? current->left->assigned += current->added : current->right->assigned += current->added;
                    } else {
                        isLeft ? current->left->added = current->added : current->right->added = current->added;
                        isLeft ? current->left->isAdded = true : current->right->isAdded = true;
                    }
                }
            }

            if (current->isAssigned) {
                if (isLeft ? current->left->isAdded : current->right->isAdded) {
                    isLeft ? current->left->isAdded = false : current->right->isAdded = false;
                }
                isLeft ? current->left->assigned = current->assigned : current->right->assigned = current->assigned;
                isLeft ? current->left->isAssigned = true : current->right->isAssigned = true;
            }
        }
    }

    void updateNode(Node *current) {
        if (current == nullptr)
            return;

        updateSide(current, true);
        updateSide(current, false);


        if (current->isReversed) {
            std::swap(current->right, current->left);
            if (current->amount == 1 || (current->isRising && current->isDecreasing)) {
                current->isDecreasing = true;
                current->isRising = true;
            } else if (current->isRising) {
                current->isDecreasing = true;
                current->isRising = false;
            } else if (current->isDecreasing) {
                current->isDecreasing = false;
                current->isRising = true;
            }
        }

        if (current->isAdded) {
            current->value += current->added;
            current->max += current->added;
            current->min += current->added;
        }

        if (current->isAssigned) {
            current->value = current->assigned;
            current->isRising = current->isDecreasing = true;
            current->max = current->min = current->assigned;
        }

        fixSum(current);
        current->isReversed = false;
        current->isAdded = false;
        current->isAssigned = false;
    }

    void fixSum(Node *current) {
        if (current == nullptr)
            return;
        if (current->isAdded)
            current->sum += current->amount * current->added;
        if (current->isAssigned)
            current->sum = current->amount * current->assigned;
    }

    void fixNode(Node *&current) {
        if (current == nullptr)
            return;

        updateNode(current);
        updateNode(current->left);
        updateNode(current->right);
        current->max = current->min = current->value;
        if (current->left != nullptr) {
            current->max = max(current->max, current->left->max);
            current->min = min(current->min, current->left->min);
            current->left->parent = current;
        }
        if (current->right != nullptr) {
            current->max = max(current->max, current->right->max);
            current->min = min(current->min, current->right->min);
            current->right->parent = current;
        }

        current->amount = getAmount(current->right) + getAmount(current->left) + 1;
        current->sum = getValue(current) + getSum(current->left) + getSum(current->right);
        current->parent = nullptr;
        pair<Monotony, Monotony> monotony = indentifyMonotony(current);

        setMonotony(monotony, current, true);
        setMonotony(monotony, current, false);
    }

    void setMonotony(pair<Monotony, Monotony> monotony, Node* current, bool isR ){
        if ((monotony.first ==  (isR ? Monotony::increase : Monotony :: decrease) || monotony.first == Monotony::permanent) &&
            ((monotony.second == (isR ? Monotony::increase : Monotony::decrease) || monotony.second == Monotony::permanent)))
            isR ? current->isRising = true : current->isDecreasing = true;
        else
            isR ? current->isRising = false : current->isDecreasing = false;

    }
    pair<Monotony, Monotony> indentifyMonotony(Node *current) {//left monotony right monotony
        if (current == nullptr)
            return make_pair(Monotony::permanent, Monotony::permanent);
        updateNode(current);
        updateNode(current->left);
        updateNode(current->right);


        long long leftMax, leftMin, rightMax, rightMin;
        leftMax = leftMin = rightMax = rightMin = current->value;

        Monotony leftMonotony = getMonotony(current->left);
        Monotony rightMonotony = getMonotony(
                current->right); //Monotony :: permanent ==||| 1 increase||| Monotony :: decrease decrease || -2 nothing
        if (current->left != nullptr) {
            leftMax = current->left->max;
            leftMin = current->left->min;
        }
        if (current->right != nullptr) {
            rightMax = current->right->max;
            rightMin = current->right->min;
        }

        if (leftMonotony == Monotony::permanent && leftMax == current->value)
            leftMonotony = Monotony::permanent;
        else if ((leftMonotony == Monotony::permanent || leftMonotony == Monotony::increase) &&
                 current->value <= leftMin)
            leftMonotony = Monotony::increase;
        else if ((leftMonotony == Monotony::permanent || leftMonotony == Monotony::decrease) &&
                 current->value >= leftMax)
            leftMonotony = Monotony::decrease;
        else leftMonotony = Monotony::noMonotony;
        if (rightMonotony == Monotony::permanent && current->value == rightMax)
            rightMonotony = Monotony::permanent;
        else if ((rightMonotony == Monotony::permanent || rightMonotony == Monotony::increase) &&
                 current->value >= rightMax)
            rightMonotony = Monotony::increase;
        else if ((rightMonotony == Monotony::permanent || rightMonotony == Monotony::decrease) &&
                 current->value <= rightMin)
            rightMonotony = Monotony::decrease;
        else rightMonotony = Monotony::noMonotony;

        return make_pair(leftMonotony, rightMonotony);
    }


    void split(Node *current, Node *&left, Node *&right, size_t index) {
        updateNode(current);
        if (current == nullptr) {
            left = nullptr;
            right = nullptr;
            return;
        }
        updateNode(current->left);
        updateNode(current->right);
        if (index <= getAmount(current->left)) {
            split(current->left, left, current->left, index);
            right = current;
        } else {
            split(current->right, current->right, right, index - 1 - getAmount(current->left));
            left = current;
        }
        fixNode(current);
    }

    Node *merge(Node *left, Node *right) {
        updateNode(left);
        updateNode(right);
        if (left == nullptr) {
            return right;
        }
        if (right == nullptr) {
            return left;
        }
        if (left->priority > right->priority) {
            left->right = merge(left->right, right);
            fixNode(left);
            return left;
        } else {
            right->left = merge(left, right->left);
            fixNode(right);
            return right;
        }
    }

    void insert(Node *&current, size_t index, long long priority, long long value) {
        Node *left, *right, *tmp = new Node(priority, value);
        tmp->index = size;
        split(current, left, right, index);
        tmp = merge(left, tmp);
        current = merge(tmp, right);
        fixNode(current);
    }

    void remove(Node *&current, size_t index) {
        updateNode(current);
        if (current == nullptr)
            return;
        if (index == getAmount(current) - 1 - getAmount(current->right)) {
            Node *tmp = current;
            current = merge(current->left, current->right);
            --size;
            delete (tmp);
            return;
        }
        if (getAmount(current->left) >= index)
            remove(current->left, index);
        else
            remove(current->right, index - getAmount(current->left) - 1);
        fixNode(current);

    }

    void print(Node *current) {
        if (current == nullptr)
            return;
        updateNode(current);
        print(current->left);
        cout << current->value << " ";
        print(current->right);
    }

    Node *findSideValue(Node *current, bool side) { //true = left false = right
        if (current == nullptr)
            return nullptr;
        updateNode(current);
        if (side ? current->left == nullptr : current->right == nullptr)
            return current;
        else
            return findSideValue(side ? current->left : current->right, side);
    }

    size_t findNodeForPerm(Node *current, long long value, size_t index, bool perm) {// true prev false next
        updateNode(current);
        if (current == nullptr)
            return 0;
        updateNode(current->left);
        updateNode(current->right);

        if (perm ? current->value >= value : current->value <= value)
            return findNodeForPerm(current->left, value, index, perm);
        else {
            if (current->right != nullptr) {
                Node *left = findSideValue(current->right, true);
                if (perm ? left->value < value : left->value > value)
                    return findNodeForPerm(current->right, value, index + 1 + getAmount(current->left), perm);
                else return index + getAmount(current->left);
            } else return index + getAmount(current->left);
        }
    }
};
