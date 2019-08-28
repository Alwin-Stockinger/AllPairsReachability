//
// Created by Alwin Stockinger.
//

#include <algorithm.basic.traversal/breadthfirstsearch.h>
#include "ReverseBFSPartitionedAPReachAlgorithm.h"

bool ReverseBFSPartitionedAPReachAlgorithm::query(Algora::Vertex *start, const Algora::Vertex *end) {

    if(start == end){
        return true;
    }

    if(!initialized){
        run();
    }

    auto* overlayStart = mainToOverlayMap[start];
    auto* overlayEnd = mainToOverlayMap[end];
    start = mainToSubMap[start];
    end = mainToSubMap[end];
    if(!start || ! end){
        return false;
    }

    //select subgraphs
    auto* startGraph = dynamic_cast<Algora::DiGraph*>(start->getParent());
    auto* endGraph = dynamic_cast<Algora::DiGraph*> (end->getParent());


    bool reachable = false;

    //same subgraph? then normal Algorithm
    if(startGraph == endGraph){

        Algora::BreadthFirstSearch<Algora::FastPropertyMap,false> bfs(false, false);

        bfs.setStartVertex(start);

        bfs.setArcStopCondition([&end, &reachable](const Algora::Arc* arc){
            reachable = arc->getHead() == end;
            return reachable;
        });

        runAlgorithm(bfs, startGraph);

        if(reachable){
            return true;
        }
    }

    std::unordered_set<const Algora::Vertex*> reachableSourceEdgeVertices{};
    std::unordered_set<Algora::Vertex*> reachableDestinationEdgeVertices{};

    auto& sourceEdgeVertices = edgeVertices[startGraph];
    auto& destinationEdgeVertices = edgeVertices[endGraph];



    Algora::BreadthFirstSearch<Algora::FastPropertyMap,false> destinationBFS(false, false);
    destinationBFS.reverseArcDirection(true);
    destinationBFS.setStartVertex(end);
    destinationBFS.setArcStopCondition([&destinationEdgeVertices, &reachableDestinationEdgeVertices, this](const Algora::Arc* arc){
        Algora::Vertex* tail = subToMainMap[arc->getTail()];    //tail because reverse BFS
       if(destinationEdgeVertices.count(tail)){
           reachableDestinationEdgeVertices.insert(mainToOverlayMap[tail]);
       }
       return reachableDestinationEdgeVertices.size() == destinationEdgeVertices.size();
    });

    if(overlayEnd){
        reachableDestinationEdgeVertices.insert(overlayEnd);
    }
    runAlgorithm(destinationBFS, endGraph);

    if(reachableDestinationEdgeVertices.empty()){
        return false;
    }



    Algora::BreadthFirstSearch<Algora::FastPropertyMap,false> sourceBFS(false, false);
    sourceBFS.setStartVertex(start);
    sourceBFS.setArcStopCondition([sourceEdgeVertices, &reachableSourceEdgeVertices, this](const Algora::Arc* arc){
       Algora::Vertex* head = subToMainMap[arc->getHead()];

       if(sourceEdgeVertices.count(head)){
           reachableSourceEdgeVertices.insert(mainToOverlayMap[head]);
       }

       return reachableSourceEdgeVertices.size() == sourceEdgeVertices.size();
    });

    if(overlayStart){
        reachableSourceEdgeVertices.insert(overlayStart);
    }
    runAlgorithm(sourceBFS, startGraph);

    if(bidirectional){
        reachable = bidirectionalOverlayBFS(reachableSourceEdgeVertices, reachableDestinationEdgeVertices);
    } else{
        reachable = normalOverlayBFS(reachableSourceEdgeVertices, reachableDestinationEdgeVertices);
    }


    return reachable;
}

void ReverseBFSPartitionedAPReachAlgorithm::initializeChildStructures() {

    diGraph->mapVertices([this](Algora::Vertex* vertex){
       auto* subVertex = mainToSubMap[vertex];
       subToMainMap[subVertex] = vertex;
    });
}

void ReverseBFSPartitionedAPReachAlgorithm::insertOverlayEdgeArcs(Algora::DiGraph *subGraph) {

    std::unordered_set<Algora::Vertex*>& overlayVertices = edgeVertices[subGraph];

    for(Algora::Vertex* sourceVertex : overlayVertices){

        Algora::Vertex* subSource = mainToSubMap[sourceVertex];
        Algora::Vertex* overlaySource = mainToOverlayMap[sourceVertex];

        std::unordered_set<Algora::Vertex*> reachableVertices{};

        Algora::BreadthFirstSearch<Algora::FastPropertyMap,false> bfs(false, false);
        bfs.setStartVertex(subSource);
        bfs.setArcStopCondition([this, &overlayVertices, &reachableVertices, &subSource](const Algora::Arc* arc){
            Algora::Vertex* head = subToMainMap[arc->getHead()];
            if(overlayVertices.count(head)){
                if(subSource != head){
                    reachableVertices.insert(mainToOverlayMap[head]);
                }
            }

            return (overlayVertices.size() - 1) == reachableVertices.size();
        });

        runAlgorithm(bfs, subGraph);


        overlayGraph->mapOutgoingArcsUntil(overlaySource, [&reachableVertices] (Algora::Arc* arc){
            Algora::Vertex* head = arc->getHead();
            reachableVertices.erase(head);
        }, [&reachableVertices](const Algora::Arc*){
            return reachableVertices.empty();
        });

        for(Algora::Vertex* reachableVertex: reachableVertices){
            overlayGraph->addArc(overlaySource, reachableVertex);
        }
    }

}

void ReverseBFSPartitionedAPReachAlgorithm::insertOverlayEdgeArcsOfNewOverlayVertex(Algora::Vertex *vertex) {

    auto* subSource = mainToSubMap[vertex];
    Algora::Vertex* overlaySource = mainToOverlayMap[vertex];

    auto* subGraph = dynamic_cast<Algora::DiGraph*> (subSource->getParent());

    std::unordered_set<Algora::Vertex*>& overlayVertices = edgeVertices[subGraph];

    std::unordered_set<Algora::Vertex*> reachableVertices{};

    Algora::BreadthFirstSearch<Algora::FastPropertyMap,false> bfs(false, false);
    bfs.setStartVertex(subSource);
    bfs.setArcStopCondition([this, &overlayVertices, &reachableVertices, &subSource](const Algora::Arc* arc){
        Algora::Vertex* head = subToMainMap[arc->getHead()];
        if(overlayVertices.count(head)){
            if(subSource != head){
                reachableVertices.insert(mainToOverlayMap[head]);
            }
        }

        return (overlayVertices.size() - 1) == reachableVertices.size();
    });

    runAlgorithm(bfs, subGraph);

    for(Algora::Vertex* reachableVertex: reachableVertices){
        overlayGraph->addArc(overlaySource, reachableVertex);
    }

    //reverse------------------
    std::unordered_set<Algora::Vertex*> reverseReachableVertices{};

    Algora::BreadthFirstSearch<Algora::FastPropertyMap,false> reverseBfs(false, false);
    reverseBfs.setStartVertex(subSource);
    reverseBfs.reverseArcDirection(true);
    reverseBfs.setArcStopCondition([this, &overlayVertices, &reverseReachableVertices, &subSource](const Algora::Arc* arc){
        Algora::Vertex* tail = subToMainMap[arc->getTail()];
        if(overlayVertices.count(tail)){
            if(subSource != tail){
                reverseReachableVertices.insert(mainToOverlayMap[tail]);
            }
        }

        return (overlayVertices.size() - 1) == reverseReachableVertices.size();
    });

    runAlgorithm(reverseBfs, subGraph);

    for(Algora::Vertex* reverseReachableVertex: reverseReachableVertices){
        overlayGraph->addArc(reverseReachableVertex, overlaySource);
    }
}

void ReverseBFSPartitionedAPReachAlgorithm::removeOverlayEdgeArcs(Algora::DiGraph *subGraph) {

    auto& overlayVertices = edgeVertices[subGraph];

    for(auto* sourceVertex : overlayVertices){

        std::unordered_set<Algora::Vertex*> unReachableVertices{};
        unReachableVertices.reserve(overlayVertices.size());
        for(auto* vertex: overlayVertices){
            unReachableVertices.insert(mainToOverlayMap[vertex]);
        }

        auto* subSource = mainToSubMap[sourceVertex];

        Algora::BreadthFirstSearch<Algora::FastPropertyMap,false> bfs(false, false);
        bfs.setStartVertex(subSource);

        bfs.setArcStopCondition([&unReachableVertices, this](const Algora::Arc* arc){
            auto* head = arc->getHead();
            unReachableVertices.erase(mainToOverlayMap[subToMainMap[head]]);

            return unReachableVertices.empty();
        });

        runAlgorithm(bfs, subGraph);

        std::vector<Algora::Arc*> arcsToRemove{};
        overlayGraph->mapOutgoingArcsUntil(mainToOverlayMap[sourceVertex], [&arcsToRemove, &unReachableVertices](Algora::Arc* arc){
            auto* head = arc->getHead();
            if(unReachableVertices.count(head)){
                arcsToRemove.push_back(arc);
            }
        },[&arcsToRemove, &unReachableVertices](const Algora::Arc* arc){
            return arcsToRemove.size() == unReachableVertices.size();
        });

        for(auto* arc : arcsToRemove){
            overlayGraph->removeArc(arc);
        }
    }

}


void ReverseBFSPartitionedAPReachAlgorithm::resetChildStructures() {
    subToMainMap.resetAll();
}

std::string ReverseBFSPartitionedAPReachAlgorithm::getBaseName() {
    unsigned long long steps = 0ULL;
    if(bidirectional){
        steps = stepSize;
    }

    return "RBFS(" + getPartitionConfiguration() + "/A=" + std::to_string(setRemovals) + "/S=" + std::to_string(steps) + ")";
}

std::string ReverseBFSPartitionedAPReachAlgorithm::getName() const noexcept {
    return std::__cxx11::string("Reverse BFS Partitioned Algorithm");
}

std::string ReverseBFSPartitionedAPReachAlgorithm::getShortName() const noexcept {
    return std::__cxx11::string("RBFSP");
}

void ReverseBFSPartitionedAPReachAlgorithm::onVertexAdd(Algora::Vertex *vertex) {
    PartitionedAPReachAlgorithm::onVertexAdd(vertex);

    auto* subVertex = mainToSubMap[vertex];

    if(subVertex){
        subToMainMap[subVertex] = vertex;
    }
}

void ReverseBFSPartitionedAPReachAlgorithm::onVertexRemove(Algora::Vertex *vertex) {

    auto* subVertex = mainToSubMap[vertex];

    if(subVertex){
        subToMainMap.resetToDefault(mainToSubMap[vertex]);
    }

    PartitionedAPReachAlgorithm::onVertexRemove(vertex);
}

void ReverseBFSPartitionedAPReachAlgorithm::addVertex(Algora::Vertex *lazyVertex, Algora::DiGraph *subGraph) {
    PartitionedAPReachAlgorithm::addVertex(lazyVertex, subGraph);

    auto* subVertex = mainToSubMap[lazyVertex];
    subToMainMap[subVertex] = lazyVertex;
}

bool ReverseBFSPartitionedAPReachAlgorithm::normalOverlayBFS(std::unordered_set<const Algora::Vertex *> sourceBorders,
                                                             std::unordered_set<Algora::Vertex *> targetBorders) {

    bool reachable = false;

    for(const Algora::Vertex* sourceEdge : sourceBorders){  //iterator is not invalidated, because only pointers to erased elements are invalidated

        Algora::BreadthFirstSearch<Algora::FastPropertyMap,false> overlayBfs(false, false);
        overlayBfs.setStartVertex(sourceEdge);

        overlayBfs.setArcStopCondition([&targetBorders, &reachable](const Algora::Arc* arc){
            reachable = targetBorders.count(arc->getHead());
            return reachable;
        });

        if(setRemovals){
            overlayBfs.onVertexDiscover([&sourceEdge, &sourceBorders](const Algora::Vertex* vertex){
                if(vertex != sourceEdge){   //otherwise iterator invalidation
                    sourceBorders.erase(vertex);
                }
                return true;
            });
        }

        runAlgorithm(overlayBfs, overlayGraph);

        if(reachable){
            return true;
        }
    }
    return reachable;
}

bool ReverseBFSPartitionedAPReachAlgorithm::bidirectionalOverlayBFS(std::unordered_set<const Algora::Vertex *> sourceBorders,
                                                                    std::unordered_set<Algora::Vertex *> targetBorders) {
    bool reachable = false;

    unsigned long long stepsTaken = 0ULL;

    Algora::FastPropertyMap<bool> sourceReachable(false);
    Algora::FastPropertyMap<bool> targetReachable(false);

    Algora::BreadthFirstSearch<Algora::FastPropertyMap,false> sourceOverlayBfs(false, false);
    std::vector<const Algora::Vertex*> sourceVector;
    sourceVector.reserve(sourceBorders.size());
    for (auto it = sourceBorders.begin(); it != sourceBorders.end(); ) {
        sourceReachable[(*it)] = true;
        sourceVector.push_back(sourceBorders.extract(it++).value());
    }
    sourceOverlayBfs.setStartVertices(sourceVector);
    sourceOverlayBfs.setArcStopCondition([&reachable, &targetReachable](const Algora::Arc* arc){
        reachable = targetReachable(arc->getHead());
        return reachable;
    });
    sourceOverlayBfs.setVertexStopCondition([&stepsTaken, &sourceReachable, this](const Algora::Vertex* vertex){
        if(!sourceReachable(vertex)){
            sourceReachable[vertex] = true;
            stepsTaken++;
        }
        return stepsTaken == stepSize;
    });


    Algora::BreadthFirstSearch<Algora::FastPropertyMap, false> targetOverlayBfs(false, false);
    std::vector<const Algora::Vertex*> targetVector;
    targetVector.reserve(targetBorders.size());
    for(auto it = targetBorders.begin(); it != targetBorders.end();){
        targetReachable[(*it)] = true;
        targetVector.push_back(targetBorders.extract(it++).value());
    }
    targetOverlayBfs.setStartVertices(targetVector);
    targetOverlayBfs.reverseArcDirection(true);
    targetOverlayBfs.setArcStopCondition([&reachable, &sourceReachable](const Algora::Arc* arc){
        reachable = sourceReachable(arc->getTail());
        return reachable;
    });
    targetOverlayBfs.setVertexStopCondition([&stepsTaken,&targetReachable, this](const Algora::Vertex* vertex){
        if(!targetReachable(vertex)){
            targetReachable[vertex] = true;
            stepsTaken++;
        }
        return stepsTaken == stepSize;
    });


    runAlgorithm(sourceOverlayBfs, overlayGraph);
    if(reachable){
        return reachable;
    }
    else if(stepsTaken != stepSize){
        return false;
    }
    stepsTaken = 0ULL;

    runAlgorithm(targetOverlayBfs, overlayGraph);
    if(reachable){
        return reachable;
    }
    else if(stepsTaken != stepSize){
        return false;
    }
    stepsTaken = 0ULL;

    while(true){
        sourceOverlayBfs.resume();
        if(reachable){
            return reachable;
        }
        else if(stepsTaken != stepSize){
            return false;
        }
        stepsTaken = 0ULL;

        targetOverlayBfs.resume();
        if(reachable){
            return reachable;
        }
        else if(stepsTaken != stepSize){
            return false;
        }
        stepsTaken = 0ULL;
    }
}


