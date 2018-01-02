#pragma once
#include <iostream>
#include <queue>
#include <vector>
#include "Network"

using std::vector;
using  std::queue;

template <typename TFlow>
class Dinic_sAalgorithm{
private:
    Network<TFlow> network_;
    int vertexNumber_;
    vector<int> dist;
    int source_, sink_;
    vector<typename Network<TFlow>::iterator> nextEdge;

    bool bfs_() { //returns false if there is no way from source to sink
        dist.assign(vertexNumber_, INF);
        dist[source_] = 0;

        queue<int> q;
        q.push(source_);
        while (!q.empty()) {
            int currentV = q.front();
            q.pop();
            for (typename Network<TFlow>::iterator i = network_.begin(currentV); i.isValid(); ++i){
                if(i.getResidualCapacity() > 0 && dist[i.getFinish()] == INF) {
                    dist[i.getFinish()] = dist[i.getStart()] + 1;
                    q.push(i.getFinish());
                }
            }
        }
        if(dist[sink_] == INF) return false;
        return true;
    };

    enum {
        INF = 2147483645
    };

    TFlow dfs_(int currentV, TFlow minFlow){
        if(currentV == sink_ || minFlow == 0 )return minFlow;

        for(typename Network<TFlow>::iterator i = nextEdge[currentV]; i.isValid();++i){
            if(dist[i.getStart()] == dist[i.getFinish()] - 1){
                TFlow minFlow_ = dfs_(i.getFinish(), std::min(minFlow, i.getResidualCapacity()));
                if(minFlow_ !=0){
                    i.pushFlow(minFlow_);
                    return minFlow_;
                }

                ++nextEdge[currentV];//no way to sink (no use of current vertex)

            }
        }

        return 0; //no way to sink
    }

    TFlow findMaxFlowLocal_() {
        TFlow maxFlow = 0;

        for(int i =0; i < vertexNumber_; ++i)
            nextEdge.push_back(network_.begin(i));

        while (bfs_()) {
            for(int i =0; i < vertexNumber_; ++i)
                nextEdge[i] = network_.begin(i);

            TFlow flow = dfs_(source_, INF);
            while(flow != 0){
                maxFlow+= flow;
                flow = dfs_(source_, INF);
            }
        }
        return maxFlow;
    }

public:
    Dinic_sAalgorithm(Network<TFlow> network):network_(network){
        vertexNumber_ = network.getVertexNumber();
        sink_ = network.getSink();
        source_ = network.getSource();
        dist.assign(vertexNumber_, INF);
    }

    TFlow findMaxFlow(){
        return findMaxFlowLocal_();
    }
};
