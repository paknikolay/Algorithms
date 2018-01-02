#pragma once
#include <vector>

using std::vector;

template <typename TFlow>
struct Edge{
    int start, finish;
    TFlow capacity, flow;
    Edge(){
        start = finish = 0;
        capacity = flow = 0;
    }
    Edge(int start_, int finish_, TFlow capacity_, TFlow flow_ = 0){
        start = start_;
        finish = finish_;
        capacity = capacity_;
        flow = flow_;
    }

};

template <typename TFlow>
class Network {
private:
    bool isDirected_;
    int vertexNumber_, sink_, source_;
    vector<Edge<TFlow>> edges_;
    vector<int> start_, previous_;
    vector<Edge<TFlow>*>reversedEdges_;
    vector<int> reversedStart_, reversedPrevious_;
    void addEdgeLocal_(int start, int finish, TFlow capacity, TFlow capacityReversed = 0, bool isAddingReversed = false) {
        if (!isAddingReversed) capacityReversed = capacity;
        edges_.push_back(Edge<TFlow>(start, finish, capacity));
        edges_.push_back(Edge<TFlow>(finish, start, isDirected_  ? isAddingReversed ? capacityReversed : 0 : capacity));
        previous_.push_back(start_[start]);
        previous_.push_back(start_[finish]);
        start_[start] = edges_.size() - 2;
        start_[finish] = edges_.size() - 1;
    }

public:
    friend class iterator;
    class iterator {
    private:
        int edgeNumber_;
        Network &network_;
        bool isReversed_;

    public:
        iterator(Network &network, int vertexNumber, bool isReversed) : network_(network) {
            isReversed_ = isReversed;
            edgeNumber_ = isReversed_ ? network.reversedStart_[vertexNumber] : network.start_[vertexNumber] ;
        }

        iterator &operator=(const iterator& iter) {
            edgeNumber_ = iter.edgeNumber_;
            network_ = iter.network_;
            isReversed_ = isReversed_;
            return *this;
        }

        iterator &operator++() {
            edgeNumber_ = isReversed_ ? network_.reversedPrevious_[edgeNumber_] : network_.previous_[edgeNumber_];
            return *this;
        }

        iterator getReversedEdge() const{
            return iterator(network_, edgeNumber_^1, true);
        }

        bool isValid() const{
            return edgeNumber_>= 0;
        }

        void pushFlow(TFlow flow) {
            if (isReversed_) {
                network_.reversedEdges_[edgeNumber_]->flow += flow;
                network_.reversedEdges_[edgeNumber_ ^ 1]->flow -= flow;
            } else {
                network_.edges_[edgeNumber_].flow += flow;
                network_.edges_[edgeNumber_ ^ 1].flow -= flow;
            }
        }

        TFlow getFlow() const {
            return isReversed_ ? network_.reversedEdges_[edgeNumber_]->flow : network_.edges_[edgeNumber_].flow;
        }

        TFlow getCapacity() const{
            return isReversed_ ? network_.reversedEdges_[edgeNumber_]->capacity:  network_.edges_[edgeNumber_].capacity;
        }

        TFlow getResidualCapacity()const {
            return getCapacity() - getFlow();
        }

        int getStart()  const{
            return isReversed_ ? network_.reversedEdges_[edgeNumber_]->start : network_.edges_[edgeNumber_].start;
        }

        int getFinish() const{
            return isReversed_ ? network_.reversedEdges_[edgeNumber_]->finish :  network_.edges_[edgeNumber_].finish;
        }

    };

    void finishNetwork(){
        reversedEdges_.assign(edges_.size(),0);
        reversedPrevious_.assign(previous_.size(), -1);
        reversedStart_.assign(start_.size(), -1);

        for (int i = 0; i < edges_.size(); ++i) {
            reversedEdges_[i] = &edges_[i];
            reversedPrevious_[i] = reversedStart_[edges_[i].finish];
            reversedStart_[edges_[i].finish] = i;
        }
    };

    Network(int vertexNumber, int sink, int source, bool isDirected = true) {
        isDirected_ = isDirected;
        vertexNumber_ = vertexNumber;
        sink_ = sink;
        source_ = source;
        start_.assign(vertexNumber, -1);
    }

    Network(const Network& other) {
        isDirected_ = other.isDirected_;
        vertexNumber_ = other.vertexNumber_;
        sink_ = other.sink_;
        source_ = other.source_;
        for(auto i: other.edges_){
            edges_.push_back(i);
        }
        for(auto i: other.start_){
            start_.push_back(i);
        }
        for(auto i: other.previous_){
            previous_.push_back(i);
        }
    }

    void addEdge(int start, int finish, TFlow capacity) {
        addEdgeLocal_(start, finish, capacity);
    }
    void addEdgeAndReversed(int start, int finish, TFlow capacity, TFlow capacityReversed) {
        addEdgeLocal_(start, finish, capacity, capacityReversed, true);
    }
    iterator begin(int vertexNumber) {
        return iterator(*this, vertexNumber, false);
    }
    iterator reversedBegin(int vertexNumber) {
        return iterator(*this, vertexNumber, true);
    }

    int getVertexNumber() const{
        return vertexNumber_;
    }

    int getSink() const{
        return sink_;
    }

    int getSource() const{
        return source_;
    }
};

