#pragma one
#include <vector>
#include <list>
#include <algorithm>
#include "Network.h"

using std::min;
using std::vector;
using std::list;
template <typename TFlow>
class RelabelToFront{
private:
    Network<TFlow>& network_;
    vector<TFlow> overflowing_;
    vector<int> heights_;
    vector<typename Network<TFlow>::iterator> iterators_;

    void push_(typename Network<TFlow>::iterator& currentEdge){
        TFlow flow = min(currentEdge.getResidualCapacity(), overflowing_[currentEdge.getStart()]);//TODO check
        currentEdge.pushFlow(flow);
        overflowing_[currentEdge.getStart()]-= flow;
        overflowing_[currentEdge.getFinish()]+= flow;
    }

    bool canPush_(const typename Network<TFlow>::iterator& currentEdge){
        return overflowing_[currentEdge.getStart()] > 0 && heights_[currentEdge.getStart()] == heights_[currentEdge.getFinish()] + 1
               && currentEdge.getResidualCapacity() > 0;
    }

    bool canRelabel_(int currentV){
        if (!(overflowing_[currentV] > 0)) return false;

        for(typename Network<TFlow>::iterator iterator = network_.begin(currentV); iterator.isValid(); ++iterator){
            if(canPush_(iterator))return false;
        }
        return true;
    }

    void relabel_(int currentV){
        int mi;
        typename Network<TFlow>::iterator iterator = network_.begin(currentV);
        for(; iterator.isValid(); ++iterator) {
            if (iterator.getResidualCapacity() > 0) {
                mi = heights_[iterator.getFinish()];
                ++iterator;
                break;
            }
        }

        for(; iterator.isValid(); ++iterator){
            if(iterator.getResidualCapacity() > 0 && mi > heights_[iterator.getFinish()])
                mi = heights_[iterator.getFinish()];
        }
        heights_[currentV]= 1 + mi;
    }

    void discharge_(int currentV){
        while(overflowing_[currentV] > 0){
            typename Network<TFlow>::iterator iterator = iterators_[currentV];
            if(!iterator.isValid()){
                relabel_(currentV);
                iterators_[currentV] = network_.begin(currentV);
            } else
            if(canPush_(iterator))
                push_(iterator);
            else ++iterators_[currentV];
        }
    }

public:
    explicit RelabelToFront(Network<TFlow>& network): network_(network) {
        overflowing_.assign(network_.getVertexNumber(), 0);
        heights_.assign(network_.getVertexNumber(), 0);
        heights_[network_.getSource()] = network_.getVertexNumber();
        for (int i = 0; i < network_.getVertexNumber(); ++i) {
            iterators_.push_back(network_.begin(i));
        }

    }

    void initialize(list<int>& vertex){
        for(typename Network<TFlow>::iterator iterator = network_.begin(network_.getSource()); iterator.isValid(); ++iterator){
            iterator.pushFlow(iterator.getCapacity());
            overflowing_[iterator.getFinish()]+= iterator.getFlow();
        }

        for (int i = 0; i < network_.getVertexNumber(); ++i) {
            if (i != network_.getSource() && i != network_.getSink())
                vertex.push_back(i);
        }
        for (int i = 0; i < network_.getVertexNumber(); ++i) {
            iterators_[i] = network_.begin(i);
        }

    }

    TFlow findMaxFlow() {
        list<int> vertex;

        initialize(vertex);

        auto currentV = vertex.begin();

        while (currentV != vertex.end()) {
            int oldHeight = heights_[*currentV];
            discharge_(*currentV);
            if (oldHeight < heights_[*currentV]) {
                vertex.push_front(*currentV);
                vertex.erase(currentV);
                currentV = vertex.begin();
            }
            ++currentV;
        }

        TFlow flow = 0;
        for (int j = 0; j < network_.getVertexNumber(); ++j) {
            for(typename Network<TFlow>::iterator iterator = network_.begin(j); iterator.isValid(); ++iterator){
                if(iterator.getFinish() == network_.getSink())flow+= iterator.getFlow();
            }
        }
        return flow;
    }
};


