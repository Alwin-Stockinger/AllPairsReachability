//
// Created by Alwin Stockinger.
//

#include <algorithm.basic.traversal/depthfirstsearch.h>
#include "DFSOverlayPDAPReachAlgorithm.h"

bool DFSOverlayPDAPReachAlgorithm::query(Algora::Vertex *start, const Algora::Vertex *end) {
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

    for(const Algora::Vertex* sourceEdge : reachableSourceEdgeVertices){  //iterator is not invalidated, because only pointers to erased elements are invalidated

        Algora::DepthFirstSearch dfs(false);
        dfs.setStartVertex(sourceEdge);

        dfs.setArcStopCondition([&reachableDestinationEdgeVertices, &reachable](const Algora::Arc* arc){
            reachable = reachableDestinationEdgeVertices.count(arc->getHead());
            return reachable;
        });

        if(setRemovals){
            dfs.onVertexDiscover([&reachableSourceEdgeVertices, &sourceEdge](const Algora::Vertex* vertex){
                if(vertex != sourceEdge){
                    reachableSourceEdgeVertices.erase(vertex);
                }
                return true;
            });
        }

        runAlgorithm(dfs, overlayGraph);

        if(reachable){
            return true;
        }
    }

    return false;
}

std::string DFSOverlayPDAPReachAlgorithm::getBaseName() {
    std::string retName = "DFSO";
    retName += "(" + getPartitionConfiguration() + "/A=" + std::to_string(setRemovals) + ")";
    retName += ": " + graphToAlgorithmMap.begin()->second->getBaseName();

    return retName;
}

std::string DFSOverlayPDAPReachAlgorithm::getName() const noexcept {
    return std::__cxx11::string("DFS Overlay Partitioned Algorithm");
}

std::string DFSOverlayPDAPReachAlgorithm::getShortName() const noexcept {
    return std::__cxx11::string("DFSO Partitioned Algorithm");
}
