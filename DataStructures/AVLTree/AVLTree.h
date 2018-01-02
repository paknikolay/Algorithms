#pragma once

#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
using std::cout;
using std::cout;

class AVLTree {
public:
    AVLTree() : root(nullptr) {};

    ~AVLTree() {
        DeleteNode(root);
    };

    int FindNode(int key) {
        if (root == nullptr)return -1;
        else
            return findNode(root, key, 0);
    }

    void Add(int key) {
        if (root == nullptr) {
            root = new Node(key, 1, 1, nullptr, nullptr);
            return;
        }
        add(key, &root);
    }

    void Remove(int key) {
        if (root == nullptr)return;;
        remove(&root, key);
    }

    void Print() {
        print(root);
    }

    void Print2() {
        print2(root);
    }

    int Findstat(int stat) {
        return findStat(root, stat);
    }

private:
    struct Node {
        int key;
        int height;
        int kol_down;
        Node *left;
        Node *right;

        Node() : key(0), height(1), kol_down(1), left(nullptr), right(nullptr) {};

        Node(int _key, int _height, int _kol_down, Node *_left, Node *_right) : key(_key), height(_height),
                                                                                kol_down(_kol_down), left(_left),
                                                                                right(_right) {};

        Node &operator=(Node &a) {
            key = a.key;
            height = a.height;
            kol_down = a.kol_down;
            left = a.left;
            right = a.right;
            return *this;
        }

    };

    int findStat(Node *cur, int stat) {
        int k = stat - getKol_down(cur->right);
        if (k == 0)return cur->key;
        else if (k < 0)return findStat(cur->right, stat);
        else return findStat(cur->left, k - 1);
    }

    Node *root;

    void print(Node *cur) {
        if (cur == nullptr)return;
        print(cur->right);
        cout << cur->key << " ";
        print(cur->left);
    }

    void DeleteNode(Node *cur) {
        if (cur == nullptr)
            return;;
        DeleteNode(cur->right);
        DeleteNode(cur->left);
        delete cur;
    }

    void print2(Node *cur) {
        if (cur == nullptr)return;
        print2(cur->right);
        cout << cur->key << " " << cur->height << " " << cur->kol_down << "\n";
        print2(cur->left);
    }


    int findNode(Node *cur, int key, int kol) {
        if (cur == nullptr)return -1;
        if (cur->key == key) return kol;
        if (cur->key < key)return findNode(cur->left, key, kol);
        return findNode(cur->right, key, 1 + kol + getKol_down(cur->left));
    }

    void remove(Node **cur, int key) {
        if ((*cur) == nullptr)return;

        if ((*cur)->key < key)
            remove(&((*cur)->left), key);

        else if ((*cur)->key > key)
            remove(&((*cur)->right), key);
        else {

            if ((*cur)->right == nullptr && (*cur)->left == nullptr) {
                delete (*cur);
                (*cur) = nullptr;
                return;
            }

            if ((*cur)->right == nullptr) {
                delete (*cur);
                (*cur) = (*cur)->left;
                return;
            }

            if ((*cur)->left == nullptr) {
                delete (*cur);
                (*cur) = (*cur)->right;
                return;
            }

            Node **left = &((*cur)->left), **right = &((*cur)->right);

            delete (*cur);

            findMin(&((*cur)->left), left, right, cur, cur, false);
        }

        balance(cur);
    }


    void findMin(Node **cur, Node **left, Node **right, Node **delNode, Node **parent, bool isRight) {
        if ((*cur)->right == nullptr) {
            Node *tmp = (*cur)->left;

            *delNode = (*cur);
            (*delNode)->left = *left;
            (*delNode)->right = *right;
            fixNode(*delNode);
            if (isRight)
                (*parent)->right = tmp;
            else
                (*parent)->left = tmp;
            return;
        } else
            findMin(&((*cur)->right), left, right, delNode, cur, true);
        balance(cur);
    }


    void add(int key, Node **cur) {
        if (*cur == nullptr) {
            *cur = new Node(key, 1, 1, nullptr, nullptr);
            return;
        } else {
            if ((*cur)->key <= key)
                add(key, &((*cur)->left));
            else
                add(key, &((*cur)->right));
        }

        balance(cur);


    }

    int getHight(Node *cur) {
        if (cur == nullptr)return 0;
        else return cur->height;

    }

    int getKol_down(Node *cur) {
        if (cur == nullptr)return 0;
        else return cur->kol_down;
    }

    void changeHeight(Node *&cur) {
        int h1 = getHight(cur->right);
        int h2 = getHight(cur->left);
        if (h1 > h2)
            cur->height = 1 + h1;
        else
            cur->height = 1 + h2;
    }

    void changeKol_down(Node *&cur) {
        int k1 = getKol_down(cur->right);
        int k2 = getKol_down(cur->left);
        cur->kol_down = 1 + k2 + k1;
    }

    void fixNode(Node *cur) {
        changeHeight(cur);
        changeKol_down(cur);
    }

    void sLeftRot(Node **cur) {
        if (cur == nullptr)return;
        Node *tmp = (*cur)->right, *tmp2 = (*cur);
        (*cur)->right = (*cur)->right->left;
        (*cur) = tmp;
        (*cur)->left = tmp2;
        fixNode((*cur)->left);
        fixNode((*cur));
    }

    void sRightRot(Node **cur) {
        if (cur == nullptr)return;
        Node *tmp = ((*cur)->left), *tmp2 = *cur;
        (*cur)->left = (*cur)->left->right;
        (*cur) = tmp;
        (*cur)->right = tmp2;
        fixNode((*cur)->right);
        fixNode((*cur));
    }

    void bLeftRot(Node **cur) {
        sRightRot(&((*cur)->right));
        sLeftRot(cur);
    }

    void bRightRot(Node **cur) {
        sLeftRot(&((*cur)->left));
        sRightRot(cur);
    }

    int getBalance(Node *cur) {
        return getHight(cur->left) - getHight(cur->right); //Left - Right
    }

    void balance(Node **cur) {
        fixNode((*cur));
        int curBalance = getBalance((*cur));
        if (abs(curBalance) < 2)return;
        if (curBalance == -2) {
            if (getBalance((*cur)->right) <= 0) {
                sLeftRot(cur);
                return;
            }
            bLeftRot(cur);
            return;
        }

        if (getBalance((*cur)->left) >= 0) {
            sRightRot(cur);
            return;
        }
        bRightRot(cur);
        return;

    }
};
