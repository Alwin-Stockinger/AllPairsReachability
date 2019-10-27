//
// Copyright (c) 2019 : Alwin Stockinger
//
//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

#include <algorithm.basic.traversal/depthfirstsearch.h>
#include "DFSOverlayPDAPReachAlgorithm.h"

bool DFSOverlayPDAPReachAlgorithm::query(Algora::Vertex *start, const Algora::Vertex *end) {

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
