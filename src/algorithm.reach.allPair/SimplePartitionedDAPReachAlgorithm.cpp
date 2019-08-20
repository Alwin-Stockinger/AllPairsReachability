//
// Created by Alwin Stockinger.
//

#include "SimplePartitionedDAPReachAlgorithm.h"

std::string SimplePartitionedDAPReachAlgorithm::getName() const noexcept {
    return std::__cxx11::string("Partitioned Graph All Pair Reachability Algorithm based on " + overlayAlgorithm -> getName()+ " and " + graphToAlgorithmMap.begin()->second->getName());
}

std::string SimplePartitionedDAPReachAlgorithm::getShortName() const noexcept {
    return std::__cxx11::string("Partitioned DAP Reach Algorithm");
}

bool SimplePartitionedDAPReachAlgorithm::query(Algora::Vertex *start, const Algora::Vertex *end) {

    if(start == end){
        return true;
    }

    if(!initialized){
        run();
    }

    start = mainToSubMap[start];
    end = mainToSubMap[end];

    if(!start || !end){
        return false;
    }

    //select subgraphs
    auto* startGraph = start->getParent();
    auto* endGraph = end->getParent();

    if(startGraph == endGraph){
        if(graphToAlgorithmMap[startGraph] -> query(start, end)){
            return true;
        }
    }
    DynamicAPReachAlgorithm* startGraphAlgorithm = graphToAlgorithmMap[startGraph];
    DynamicAPReachAlgorithm* endGraphAlgorithm = graphToAlgorithmMap[endGraph];

    std::unordered_set<Algora::Vertex *>& startEdgeVertices = edgeVertices[startGraph];
    std::unordered_set<Algora::Vertex *> endEdgeVertices = edgeVertices.getValue(endGraph);    //must be copy, otherwise can't erase vertices from set


    for (Algora::Vertex *outVertex : startEdgeVertices) {


        if (startGraphAlgorithm->query(start, mainToSubMap[outVertex])) {

            for(auto it = endEdgeVertices.begin(); it != endEdgeVertices.end();){

                Algora::Vertex* inVertex = *it;

                if( overlayAlgorithm->query(mainToOverlayMap[outVertex], mainToOverlayMap[inVertex])){

                    if( endGraphAlgorithm->query(mainToSubMap[inVertex], end)){
                        return true;
                    }
                    else{
                        it = endEdgeVertices.erase(it);    //exclude for future queries, because dead-end
                    }
                }
                else it++;
            }
        }
    }

    //return that no connection was found
    return false;
}

SimplePartitionedDAPReachAlgorithm::~SimplePartitionedDAPReachAlgorithm() {
    delete overlayAlgorithm;
}

void SimplePartitionedDAPReachAlgorithm::initializeChildStructures() {
    PartitionedDynamicAPReachAlgorithm::initializeChildStructures();
    overlayAlgorithm = createOverlayAlgorithm();
    overlayAlgorithm->setGraph(overlayGraph);
    overlayAlgorithm->run();
}

void SimplePartitionedDAPReachAlgorithm::resetChildStructures() {
    delete overlayAlgorithm;
    PartitionedDynamicAPReachAlgorithm::resetChildStructures();
}
