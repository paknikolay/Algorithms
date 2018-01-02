#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <limits>

using std::vector;
using std::cin;
using std::cout;
using std::string;
using std::map;
using std::pair;
using std:: make_pair;
using std::swap;

struct SuffixAutomatonNode {
    SuffixAutomatonNode *suffLink;
    map<char, SuffixAutomatonNode *> edges;
    size_t length;
    int number;
    bool isTerminal;

    SuffixAutomatonNode(int number_):suffLink(nullptr), length(0), number(number_), isTerminal(false) {
    }

    static SuffixAutomatonNode* clone(SuffixAutomatonNode* suffixAutomatonNode, int number){
        SuffixAutomatonNode* newSuffixAutomatonNode = new SuffixAutomatonNode(number);
        newSuffixAutomatonNode->suffLink = suffixAutomatonNode->suffLink;
        newSuffixAutomatonNode->length = suffixAutomatonNode->length;

        for (auto iter: suffixAutomatonNode->edges) {
            newSuffixAutomatonNode->edges[iter.first] = iter.second;
        }
        return newSuffixAutomatonNode;
    }
};


class SuffixAutomaton {
private:
    SuffixAutomatonNode* root_;
    SuffixAutomatonNode* last_;
    int currentNumber_;

    void addSymbol(char symbol) {
        SuffixAutomatonNode* curSuffixAutomaton = last_;
        SuffixAutomatonNode* newSuffixAutomaton = new SuffixAutomatonNode(currentNumber_++);
        newSuffixAutomaton->length = last_->length + 1;

        while (curSuffixAutomaton != nullptr && curSuffixAutomaton->edges.find(symbol) == curSuffixAutomaton->edges.end()) {
            curSuffixAutomaton->edges[symbol] = newSuffixAutomaton;
            curSuffixAutomaton = curSuffixAutomaton->suffLink;
        }

        if (curSuffixAutomaton == nullptr) {
            newSuffixAutomaton->suffLink = root_;
        } else {
            SuffixAutomatonNode* nexSuffixAutomaton = curSuffixAutomaton->edges[symbol];
            if (curSuffixAutomaton->length + 1 == nexSuffixAutomaton->length) {
                newSuffixAutomaton->suffLink = nexSuffixAutomaton;
            } else {
                SuffixAutomatonNode* clone = SuffixAutomatonNode::clone(nexSuffixAutomaton, currentNumber_++);
                clone->length = curSuffixAutomaton->length + 1;

                nexSuffixAutomaton->suffLink = clone;
                newSuffixAutomaton->suffLink = clone;

                while (curSuffixAutomaton != nullptr && curSuffixAutomaton->edges.find(symbol) != curSuffixAutomaton->edges.end() &&
                       curSuffixAutomaton->edges.find(symbol)->second->number == nexSuffixAutomaton->number) {
                    curSuffixAutomaton->edges[symbol] = clone;
                    curSuffixAutomaton = curSuffixAutomaton->suffLink;
                }
            }
        }
        last_ = newSuffixAutomaton;
    }


    void makeTerminalSuffixAutomaton_(SuffixAutomatonNode* curSuffixAutomaton) const {
        if (curSuffixAutomaton->suffLink == nullptr || curSuffixAutomaton->suffLink == curSuffixAutomaton) {
            return;
        }
        curSuffixAutomaton->isTerminal = true;
        makeTerminalSuffixAutomaton_(curSuffixAutomaton->suffLink);
    }

    void findTerminalSuffixAutomaton_(SuffixAutomatonNode* curSuffixAutomatonNode, size_t index, const string& str) const{
        if (index == str.length()) {
            makeTerminalSuffixAutomaton_(curSuffixAutomatonNode);
            return;
        }
        findTerminalSuffixAutomaton_(curSuffixAutomatonNode->edges[str[index]], index + 1, str);
    }

public:
    SuffixAutomaton(const string& str) {
        currentNumber_ = 0;
        root_ = new SuffixAutomatonNode(currentNumber_++);
        last_ = root_;

        for (size_t i = 0; i < str.length(); ++i) {
            addSymbol(str[i]);
        }

        findTerminalSuffixAutomaton_(root_, 0, str);
    }

    const SuffixAutomatonNode *getRoot() const {
        return root_;
    }
};

