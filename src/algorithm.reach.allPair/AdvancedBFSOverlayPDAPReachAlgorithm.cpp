//
// Created by Alwin Stockinger.
//

#include <algorithm.basic.traversal/breadthfirstsearch.h>
#include "AdvancedBFSOverlayPDAPReachAlgorithm.h"

bool AdvancedBFSOverlayPDAPReachAlgorithm::query(Algora::Vertex *start, const Algora::Vertex *end) {
    if(!initialized){
        run();
    }
    start = mainToSubMap[start];
    end = mainToSubMap[end];

    //select subgraphs
    auto* startGraph = start->getParent();
    auto* endGraph = end->getParent();

    //same subgraph? then normal Algorithm
    if(startGraph == endGraph){
        return graphToAlgorithmMap[startGraph] -> query(start, end);
    }
    else{
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

        for(const Algora::Vertex* sourceEdge : reachableSourceEdgeVertices){  //iterator is not invalidated, because only pointers to erased elements are invalidated

            Algora::BreadthFirstSearch<Algora::FastPropertyMap,false> bfs(false, false);
            bfs.setStartVertex(sourceEdge);

            bfs.setArcStopCondition([&reachableDestinationEdgeVertices, &reachable](const Algora::Arc* arc){
                reachable = reachableDestinationEdgeVertices.count(arc->getHead());
                return reachable;
            });

            bfs.onVertexDiscover([&reachableSourceEdgeVertices, &sourceEdge](const Algora::Vertex* vertex){
                if(vertex != sourceEdge){
                    reachableSourceEdgeVertices.erase(vertex);
                }
                return true;
            });

            runAlgorithm(bfs, overlayGraph);

            if(reachable){
                return true;
            }
        }

        return false;
    }

}

std::string AdvancedBFSOverlayPDAPReachAlgorithm::getBaseName() {
    return "AdvancedBFS: " + graphToAlgorithmMap.begin()->second->getBaseName();
}

std::string AdvancedBFSOverlayPDAPReachAlgorithm::getName() const noexcept {
    return std::__cxx11::string("Advanced BFS Overlay Partitioned Algorithm");
}

std::string AdvancedBFSOverlayPDAPReachAlgorithm::getShortName() const noexcept {
    return std::__cxx11::string("ABFS Partitioned Algorithm");
}
