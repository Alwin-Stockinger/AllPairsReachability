//
// Created by Alwin Stockinger.
//

#include <algorithm.basic.traversal/breadthfirstsearch.h>
#include "SimpleBFSOverlayPDAPReachAlgorithm.h"


bool SimpleBFSOverlayPDAPReachAlgorithm::query(Algora::Vertex *start, const Algora::Vertex *end) {
    if(!initialized){
        run();
    }
    start = mainToSubMap[start];
    end = mainToSubMap[end];

    //select subgraphs
    auto* startGraph = start->getParent();
    auto* endGraph = end->getParent();

    //same subgraph? then normal Algorithm
    //same subgraph? then normal Algorithm
    if(startGraph == endGraph){
        if(graphToAlgorithmMap[startGraph] -> query(start, end)){
            return true;
        }
    }

    std::unordered_set<const Algora::Vertex*> reachableSourceEdgeVertices{};
    std::unordered_set<Algora::Vertex*> reachableDestinationEdgeVertices{};
    auto& sourceEdgeVertices = edgeVertices[startGraph];
    auto& destinationEdgeVertices = edgeVertices[endGraph];

    auto* sourceAlgorithm = graphToAlgorithmMap[startGraph];
    auto* destinationAlgorithm = graphToAlgorithmMap[endGraph];

    for(Algora::Vertex* destinationEdge: destinationEdgeVertices){
        if(destinationAlgorithm->query(mainToSubMap[destinationEdge], end)){
            reachableDestinationEdgeVertices.insert(mainToOverlayMap[destinationEdge]);
        }
    }
    if(reachableDestinationEdgeVertices.empty()){    //no tree search necessary
        return false;
    }

    for(Algora::Vertex* sourceEdge: sourceEdgeVertices){
        if(sourceAlgorithm->query(start, mainToSubMap[sourceEdge])){
            const Algora::Vertex* vertex = mainToOverlayMap[sourceEdge];
            reachableSourceEdgeVertices.insert(vertex);
        }
    }

    bool reachable = false;

    for(const Algora::Vertex* sourceEdge : reachableSourceEdgeVertices){

        Algora::BreadthFirstSearch<Algora::FastPropertyMap,false> bfs(false, false);
        bfs.setStartVertex(sourceEdge);

        bfs.setArcStopCondition([&reachableDestinationEdgeVertices, &reachable](const Algora::Arc* arc){
            reachable = reachableDestinationEdgeVertices.count(arc->getHead());
            return reachable;
        });

        runAlgorithm(bfs, overlayGraph);

        if(reachable){
            return true;
        }
    }

    return false;
}

std::string SimpleBFSOverlayPDAPReachAlgorithm::getName() const noexcept {
    return std::__cxx11::string("Simple BFS Overlay Partitioned Algorithm");
}

std::string SimpleBFSOverlayPDAPReachAlgorithm::getShortName() const noexcept {
    return std::__cxx11::string("SBFSO Partitioned Algorithm");
}

std::string SimpleBFSOverlayPDAPReachAlgorithm::getBaseName() {
    std::string retName = "SBFSO";
    retName += "(k=" + std::to_string(k);
    retName += "/d=" + std::to_string(depth);
    retName += "): " + graphToAlgorithmMap.begin()->second->getBaseName();

    return retName;
}