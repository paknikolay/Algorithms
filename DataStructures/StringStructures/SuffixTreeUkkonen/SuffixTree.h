#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <limits>
#include <assert.h>

using std::vector;
using std::cin;
using std::cout;
using std::string;
using std::map;
using std::pair;
using std:: make_pair;
using std::swap;

struct SuffixTreeNode{
    SuffixTreeNode* suffixLink;
    SuffixTreeNode* parent;
    map<char, SuffixTreeNode*> edges;
    size_t start, end;//[start, end]

    SuffixTreeNode(long start_, SuffixTreeNode* parent_){
        start = start_;
        end = std::numeric_limits<int>::max();
        suffixLink =  nullptr;
        parent = parent_;
    }

    //return if can go from position by symbol and change position to position where it will go
    bool goIfPossible(char symbol, unsigned int& position, SuffixTreeNode*& curPos,const string & str) const {
        if (position + start + 1 <= end) {
            if (str[position + start + 1] == symbol) {
                ++position;
                return true;
            } else {
                return false;
            }
        }
        //need to find among edges
        auto iter = edges.find(symbol);
        if (iter != edges.end()) {
            curPos = iter->second;
            position = 0;
            return true;
        } else {
            return false;
        }
    }
};

class SuffixTree{
private:
    string str_;
    SuffixTreeNode* root_;
    SuffixTreeNode* lastNotLeaf_;
    unsigned int posLastNotLeaf_;
    const char END_OF_STRING_SYMBOL= '$';

    void addNewNode_(unsigned int nextPos, bool isReadyToAdd = false) {
        if (lastNotLeaf_ == nullptr) {//root
            lastNotLeaf_ = root_;
        }
        char symbol = str_[nextPos];

        if (lastNotLeaf_->goIfPossible(symbol, posLastNotLeaf_, lastNotLeaf_, str_)) {
            return;
        } else {
            //making new node
            if (!isReadyToAdd) {
                getCurNode_(lastNotLeaf_, posLastNotLeaf_, lastNotLeaf_);
            }

            SuffixTreeNode* newNode = new SuffixTreeNode(nextPos, lastNotLeaf_);

            lastNotLeaf_->edges.insert(make_pair(symbol, newNode));

            if (lastNotLeaf_ != root_) {
                if (lastNotLeaf_->suffixLink == nullptr) {
                    lastNotLeaf_->suffixLink = findSuffLink_(lastNotLeaf_->parent->suffixLink, symbol, lastNotLeaf_,
                                                             lastNotLeaf_->end - lastNotLeaf_->start
                                                             + (lastNotLeaf_->parent == root_ ? 0 : 1)
                    );
                }
                //making new node in suffLink till the root
                lastNotLeaf_ = lastNotLeaf_->suffixLink;
                posLastNotLeaf_ = lastNotLeaf_->end - lastNotLeaf_->start;
                addNewNode_(nextPos, true);
            }
        }
    }

    //split node if need according to position; startNode ---->  startNode' -> startNode''
    //returns startNode''; parent = startNode'
    //position can be -1, it means no need to split edge
    SuffixTreeNode* getCurNode_(SuffixTreeNode* startNode, int position, SuffixTreeNode*& parent) {
        if(position < 0){
            parent = startNode;
            return startNode;
        }
        if (position + startNode->start == startNode->end) {
            parent = startNode;
            return startNode;
        } else {
            if(startNode== root_)
                return root_;

            SuffixTreeNode *newNode = new SuffixTreeNode(startNode->start, startNode->parent);
            newNode->end = startNode->start + position;

            startNode->parent->edges[str_[startNode->start]] = newNode;
            startNode->parent = newNode;
            startNode->start =startNode->start + position + 1;

            newNode->edges.insert(make_pair(str_[startNode->start], startNode));
            parent = newNode;
            return startNode;

        }
    }

    SuffixTreeNode* findSuffLink_(SuffixTreeNode* startNode, char symbol, SuffixTreeNode* currNode, int amountOfSymbol) {
        if(amountOfSymbol == 0){
            return root_;
        }
        if (currNode == nullptr || currNode == root_) {
            currNode =  root_;
        }
        if (startNode == nullptr || startNode == root_) {
            startNode = root_;
        }
        int currentAmountOfSym = 0;
        startNode = startNode->edges[str_[currNode->start + (currNode->parent == root_ ? 1 : 0)]];//if root_ need to skip 1 symbol from beginning

        //skipping nodes
        while (currentAmountOfSym + startNode->end - startNode->start + 1 < static_cast<size_t >(amountOfSymbol)) {
            currentAmountOfSym += startNode->end - startNode->start + 1;
            startNode = startNode->edges[str_[currNode->start + currentAmountOfSym  + (currNode->parent == root_ ? 1 : 0)]];
        }
        SuffixTreeNode* parentNode;
        getCurNode_(startNode, amountOfSymbol - currentAmountOfSym - 1, parentNode);
        return parentNode;
    }
public:
    SuffixTree(const string& str) {
        str_ = str;
        str_.push_back(END_OF_STRING_SYMBOL);
        root_ = new SuffixTreeNode(0, root_);
        root_->parent = root_;
        root_->suffixLink = root_;
        root_->end = 0;
        lastNotLeaf_ = root_;
        posLastNotLeaf_ = 0;

        for (size_t i = 0; i < str_.size(); ++i) {
            addNewNode_(i);
        }
    }

    const SuffixTreeNode* getRoot() const{
        return root_;
    }

};
