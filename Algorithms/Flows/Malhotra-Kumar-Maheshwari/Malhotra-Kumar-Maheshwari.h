#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
#include <stack>
#include "Network.h"

using std::stack;
using std::vector;
using std::queue;
using std::min;

template <typename TFlow>
class MKM {
private:
    vector<TFlow> potentialIn_, potentialOut_;
    vector<bool> deletedVertices_;//true = deleted
    Network<TFlow>& network_;
    vector<int> distances_;
    vector<typename Network<TFlow>::iterator> nextEdge_;
    vector<typename Network<TFlow>::iterator> previousEdge_;
    stack<int> deletingVertices_;

    bool bfs_() { //returns false if there is no way from source to sink
        distances_.assign(network_.getVertexNumber(), INF);
        distances_[network_.getSource()] = 0;

        queue<int> q;
        q.push(network_.getSource());
        while (!q.empty()) {
            int currentV = q.front();
            q.pop();
            for (auto i = network_.begin(currentV); i.isValid(); ++i) {
                if (i.getResidualCapacity() > 0 &&
                    distances_[i.getFinish()] == INF) {
                    distances_[i.getFinish()] = distances_[i.getStart()] + 1;
                    q.push(i.getFinish());
                }
            }
        }
        if (distances_[network_.getSink()] == INF) return false;
        return true;
    };

    enum {
        INF = 2147483645
    };

    void deleteVertices_() {
        int currentV = deletingVertices_.top();
        deletingVertices_.pop();
        while (!deletingVertices_.empty() && deletedVertices_[currentV]) {
            currentV = deletingVertices_.top();
            deletingVertices_.pop();
        }
        if (deletedVertices_[currentV])
            return;
        deletedVertices_[currentV] = true;

        for (auto i = network_.begin(currentV); i.isValid(); ++i) {
            if (distances_[i.getFinish()] > distances_[i.getStart()]) {
                potentialIn_[i.getFinish()] -= i.getResidualCapacity();
                if (getPotential_(i.getFinish()) == 0) {
                    deletingVertices_.push(i.getFinish());

                }
            }
        }
        for (auto i = network_.reversedBegin(currentV); i.isValid(); ++i) {
            if (distances_[i.getFinish()] > distances_[i.getStart()]) {
                potentialOut_[i.getStart()] -= i.getResidualCapacity();
                if (getPotential_(i.getStart()) == 0) {
                    deletingVertices_.push(i.getStart());
                }
            }
        }
    }

    void deleteDeletingVertices_() {
        while (!deletingVertices_.empty()) {
            deleteVertices_();
        }
    }

    void makePotentials_() {
        for (int i = 0; i < network_.getVertexNumber(); ++i) {
            potentialOut_[i] = 0;
            potentialIn_[i] = 0;
        }
        for (int i = 0; i < network_.getVertexNumber(); ++i) {
            if (!deletedVertices_[i])
                for (auto j = network_.begin(i); j.isValid(); ++j) {
                    if (!deletedVertices_[j.getFinish()] && distances_[i] < distances_[j.getFinish()]) {
                        potentialOut_[i] += j.getResidualCapacity();
                        potentialIn_[j.getFinish()] += j.getResidualCapacity();
                    }
                }
        }
        for (int k = 0; k < network_.getVertexNumber(); ++k) {
            if (distances_[k] == INF || getPotential_(k) == 0) {
                deletingVertices_.push(k);
            }
        }

        deleteDeletingVertices_();
    }

    TFlow getPotential_(int i) const{
        if (i == network_.getSink())return potentialIn_[i];
        if (i == network_.getSource())return potentialOut_[i];
        return min(potentialIn_[i], potentialOut_[i]);
    }

    TFlow findMinPotential_() {
        int i = 0;
        for (; i < network_.getVertexNumber(); ++i) {
            if (!deletedVertices_[i])break;
        }
        if (i == network_.getVertexNumber()) return -1;

        TFlow mi = getPotential_(i);
        int miInd = i;

        ++i;
        for (; i < network_.getVertexNumber(); ++i) {
            if (!deletedVertices_[i] && mi > getPotential_(i)) {
                mi = getPotential_(i);
                miInd = i;
            }
        }

        return miInd;
    }

    void push_(int minV, TFlow flow, bool isToTheSink = true) {
        for (; isToTheSink ? nextEdge_[minV].isValid() : previousEdge_[minV].isValid(); isToTheSink ? ++nextEdge_[minV] : ++previousEdge_[minV]) {
            if (!deletedVertices_[isToTheSink ? nextEdge_[minV].getFinish() : previousEdge_[minV].getStart()]
                && (isToTheSink ? distances_[minV] < distances_[nextEdge_[minV].getFinish()] : distances_[minV] > distances_[previousEdge_[minV].getStart()]) &&
                (isToTheSink ? nextEdge_[minV].getResidualCapacity() > 0 : previousEdge_[minV].getResidualCapacity() > 0)) {
                TFlow tmpFlow = min(flow, isToTheSink ? nextEdge_[minV].getResidualCapacity()
                                                      : previousEdge_[minV].getResidualCapacity());
                isToTheSink ? nextEdge_[minV].pushFlow(tmpFlow) : previousEdge_[minV].pushFlow(tmpFlow);
                potentialOut_[isToTheSink ? nextEdge_[minV].getStart() : previousEdge_[minV].getStart()] -= tmpFlow;
                potentialIn_[isToTheSink ? nextEdge_[minV].getFinish() : previousEdge_[minV].getFinish()] -= tmpFlow;

                if (potentialOut_[isToTheSink ? nextEdge_[minV].getStart() : previousEdge_[minV].getStart()] == 0)
                    deletingVertices_.push(isToTheSink ? nextEdge_[minV].getStart() : previousEdge_[minV].getStart());
                if (potentialIn_[isToTheSink ? nextEdge_[minV].getFinish() : previousEdge_[minV].getFinish()] == 0)
                    deletingVertices_.push(isToTheSink ? nextEdge_[minV].getFinish() : previousEdge_[minV].getFinish());
                push_(isToTheSink ? nextEdge_[minV].getFinish() : previousEdge_[minV].getStart(), tmpFlow, isToTheSink);
                flow -= tmpFlow;
                if (flow == 0)break;
            }
        }
    }


    void pushToMinV_(int minV, TFlow flow) {
        push_(minV, flow, false);
    }

    void pushFromMinV_(int minV, TFlow flow) {
        push_(minV, flow, true);
    }
public:
    explicit MKM(Network<TFlow> &network) : network_(network) {
        network_.finishNetwork();
        potentialIn_.assign(network_.getVertexNumber(), 0);
        potentialOut_.assign(network_.getVertexNumber(), 0);
        deletedVertices_.assign(network_.getVertexNumber(), false);

    };

    TFlow findMaxFlow() {
        TFlow maxFlow = 0;
        for (int i = 0; i < network_.getVertexNumber(); ++i) {
            nextEdge_.push_back(network_.begin(i));
            previousEdge_.push_back(network_.reversedBegin(i));
        }
        while (bfs_()) {
            deletedVertices_.assign(network_.getVertexNumber(), false);
            makePotentials_();
            for (int i = 0; i < network_.getVertexNumber(); ++i) {
                auto h = network_.begin(i);
                nextEdge_[i] = h;
                previousEdge_[i] = network_.reversedBegin(i);
            }
            int minV = findMinPotential_();
            TFlow tmpFlow = getPotential_(minV);
            while (tmpFlow != 0) {
                maxFlow += tmpFlow;
                pushFromMinV_(minV, tmpFlow);
                pushToMinV_(minV, tmpFlow);
                deletingVertices_.push(minV);
                deleteDeletingVertices_();
                minV = findMinPotential_();
                if (minV == -1)break;
                tmpFlow = getPotential_(minV);
            }
        }
        return maxFlow;
    }
};


