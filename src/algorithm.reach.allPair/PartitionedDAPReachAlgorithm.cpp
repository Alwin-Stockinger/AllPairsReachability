//
// Created by Alwin Stockinger.
//

#include "PartitionedDAPReachAlgorithm.h"

template<typename T>
void PartitionedDAPReachAlgorithm<T>::run() {
    overlayAlgorithm -> run();
    for(T* algorithm : apAlgorithms){
        algorithm -> run();
    }

}

template<typename T>
std::string PartitionedDAPReachAlgorithm<T>::getName() const noexcept {
    return std::__cxx11::string("Partitioned Graph All Pair Reachability Algorithm");
}

template<typename T>
std::string PartitionedDAPReachAlgorithm<T>::getShortName() const noexcept {
    return std::__cxx11::string("Partitioned DAP Reach Algo");
}

template<typename T>
bool PartitionedDAPReachAlgorithm<T>::query(const Algora::Vertex *start, const Algora::Vertex *end) {

    DynamicAPReachAlgorithm* startGraphAlgorithm = nullptr;
    DynamicAPReachAlgorithm* endGraphAlgorithm = nullptr;

    //select subgraphalgorithms
    //optimizable with breaks
    for( DynamicAPReachAlgorithm* apAlgorithm: apAlgorithms){
        if(apAlgorithm->containsVertex(start)){
            startGraphAlgorithm = apAlgorithm;
        }
        if(apAlgorithm->containsVertex(end)) {
            endGraphAlgorithm = apAlgorithm;
        }
    }

    if(!startGraphAlgorithm || !endGraphAlgorithm){
        //TODO throw exception?
        return false;
    }

    //same subgraph? then normal Algorithm
    if(startGraphAlgorithm == endGraphAlgorithm){
        return startGraphAlgorithm -> query(start, end);
    }
    else {

        std::vector<Algora::Vertex *> reachableEdges;

        std::vector<Algora::Vertex *> startEdgeVertices = edgeVertices.at(startGraphAlgorithm);

        //find outgoing vertices
        for (Algora::Vertex *vertex : startEdgeVertices) {
            if (startGraphAlgorithm->query(start, vertex)) {
                reachableEdges.push_back(vertex);
            }
        }

        std::vector<Algora::Vertex *> endEdgeVertices = edgeVertices.at(endGraphAlgorithm);
        std::vector<Algora::Vertex *> overlayConnectedVertices;


        //find connections in overlay graph
        for(Algora::Vertex *startVertex : reachableEdges){
            for(Algora::Vertex *endVertex : endEdgeVertices){
                if( overlayAlgorithm.query(startVertex, endVertex)){
                    overlayConnectedVertices.push_back(endVertex);
                }
            }
        }

        //check for connections
        for(Algora::Vertex *overlayVertex : overlayConnectedVertices){
            if( endGraphAlgorithm->query(overlayVertex, end)){
                return true;
            }
        }

        //return that no connection was found
        return false;
    }

}

template<typename T>
PartitionedDAPReachAlgorithm<T>::PartitionedDAPReachAlgorithm(std::vector<Algora::DiGraph *>* graphs,
                                                              Algora::DiGraph* connectorGraph) : DynamicAPReachAlgorithm() {
    setGraphs(graphs, connectorGraph);
}

template<typename T>
PartitionedDAPReachAlgorithm<T>::~PartitionedDAPReachAlgorithm() {
    deleteAlgorithms();
}

template<typename T>
void PartitionedDAPReachAlgorithm<T>::deleteAlgorithms() {
    for(DynamicAPReachAlgorithm* algorithm: apAlgorithms){
        delete algorithm;
    }

    delete overlayAlgorithm;

    apAlgorithms.clear();

    edgeVertices.clear();
}

template<typename T>
void
PartitionedDAPReachAlgorithm<T>::setGraphs(std::vector<Algora::DiGraph *> *graphs, Algora::DiGraph *connectorGraph) {
    //delete old Algorithms
    deleteAlgorithms();


    //create new Algorithms
    overlayAlgorithm = new T(connectorGraph);
    for (Algora::DiGraph *graph : *graphs) {
        DynamicAPReachAlgorithm* algorithm = new T(graph);
        apAlgorithms.push_back( algorithm);

        std::vector<Algora::Vertex*> edgesOfGraph;
        graph->mapVertices([&edgesOfGraph, &connectorGraph](Algora::Vertex *vertex) {
            if( connectorGraph->containsVertex(vertex)){
                edgesOfGraph.push_back(vertex);
            }
        });
        edgeVertices.insert({algorithm, edgesOfGraph});
    }
}
