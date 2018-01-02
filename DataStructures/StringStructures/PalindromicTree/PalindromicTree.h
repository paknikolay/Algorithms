#pragma once
#include <iostream>
#include <string>
#include <map>
#include <queue>
#include <set>

using std::set;
using std::queue;
using std::map;
using std::string;
using std::cout;
using std::cin;

struct Node{
    int length, end;
    int occurTosufLink;
    map<char, Node*> edges;
    Node* suffLink;
    char symbol;
    int number;
    Node* parent;

    Node(int num){
        number = num;
        suffLink = nullptr;
        length = 0;
        end = 0;
        occurTosufLink = 0;
        parent = nullptr;
    }
    bool operator< (const Node& node2){
        return length == node2.length ? symbol > node2.symbol : length > node2.length;
    }

};

class Comp {
public:
    bool operator()(Node *node1, Node *node2) {
        return node1->length == node2->length ? node1->symbol == node2->symbol ?
                                                node1->number > node2->number
                                                                               : node1->symbol > node2->symbol :
               node1->length > node2->length;
    }

};



class PalindromicTree{
private:
    Node* rootOdd_;
    Node* rootEven_;
    Node* maxLastPalSuff_;
    int curNumber;
    string s_;
    void addSymbol_(int ind, Node* currentNode) {
        if (currentNode == rootOdd_) {
            if (currentNode->edges.find(s_[ind]) == currentNode->edges.end()) {
                Node *newNode = new Node(curNumber++);
                maxLastPalSuff_ = newNode;
                newNode->parent = currentNode;
                newNode->occurTosufLink = 1;
                newNode->length = currentNode->length + 2;
                newNode->end = ind;
                newNode->symbol = s_[ind];
                currentNode->edges[s_[ind]] = newNode;
                newNode->suffLink = rootEven_;
                return;
            } else {
                maxLastPalSuff_ = currentNode->edges.find(s_[ind])->second;
                ++maxLastPalSuff_->occurTosufLink;
                return;
            }
        }

        int curInd = ind - currentNode->length - 1;
        if (curInd < 0) {
            addSymbol_(ind, currentNode->suffLink);
            return;
        }


        if (s_[curInd] == s_[ind]) {
            if (currentNode->edges.find(s_[ind]) != currentNode->edges.end()) {
                maxLastPalSuff_ = currentNode->edges.find(s_[ind])->second;
                ++maxLastPalSuff_->occurTosufLink;
            } else {
                Node *newNode = new Node(curNumber++);
                maxLastPalSuff_ = newNode;
                newNode->parent = currentNode;
                newNode->occurTosufLink = 1;
                newNode->length = currentNode->length + 2;
                newNode->end = ind;
                newNode->symbol = s_[ind];
                currentNode->edges[s_[ind]] = newNode;
                newNode->suffLink = findSuffLink(ind, currentNode->suffLink);
            }
        } else {
            addSymbol_(ind, currentNode->suffLink);
            return;
        }
    }

    Node* findSuffLink(int ind, Node* currentNode) {
        if (currentNode == rootOdd_) {
            return currentNode->edges.find(s_[ind])->second;
        }
        int curInd = ind - currentNode->length - 1;
        if (curInd < 0) {
            return findSuffLink(ind, currentNode->suffLink);
        }

        if (s_[curInd] == s_[ind] && currentNode->edges.find(s_[ind]) != currentNode->edges.end()) {
            return currentNode->edges.find(s_[ind])->second;
        } else {
            return findSuffLink(ind, currentNode->suffLink);
        }
    }

    void calculateOccur_(){
        queue<Node*> currNodes;
        currNodes.push(rootEven_);
        currNodes.push(rootOdd_);
        set<Node*, Comp> nodesToHandle;

        while(!currNodes.empty()){
            Node* curNode = currNodes.front();
            currNodes.pop();
            for(auto iter: curNode->edges){
                currNodes.push(iter.second);
            }
            nodesToHandle.insert(curNode);
        }

        for(auto curNode : nodesToHandle){
            curNode->suffLink->occurTosufLink+= curNode->occurTosufLink;
        }
    }

    void remove(Node* curNode){
        for (auto iter: curNode->edges) {
            remove(iter.second);
        }
        delete curNode;
    }
public:

    PalindromicTree(const string& s){
        curNumber = 0;
        s_ = s;
        rootEven_ = new Node(curNumber++);
        rootOdd_ = new Node(curNumber++);
        rootOdd_->suffLink = rootOdd_;
        rootOdd_->length = -1;
        rootOdd_->end = -1;
        rootEven_->end = -2;
        rootEven_->suffLink = rootOdd_;

        maxLastPalSuff_ = rootOdd_;
        for (int i = 0; i < s.size(); ++i) {
            addSymbol_(i, maxLastPalSuff_);
        }
        calculateOccur_();
    }

    ~PalindromicTree(){
        remove(rootOdd_);
        remove(rootEven_);
    }

    Node* getRootEven(){
        return rootEven_;
    }
    Node* getRootOdd(){
        return rootOdd_;
    }
};
