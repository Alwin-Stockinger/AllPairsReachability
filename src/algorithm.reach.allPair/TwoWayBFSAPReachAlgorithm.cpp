//
// Created by Alwin Stockinger.
//

#include <algorithm.basic.traversal/breadthfirstsearch.h>
#include <property/fastpropertymap.h>
#include <unordered_set>
#include "TwoWayBFSAPReachAlgorithm.h"

bool TwoWayBFSAPReachAlgorithm::query(Algora::Vertex *start, const Algora::Vertex *end) {

    if(start == end){
        return true;
    }

    bool reachable = false;

    unsigned long long stepsTaken = 0ULL;

    std::unordered_set<const Algora::Vertex*> sourceSet = {start};
    std::unordered_set<const Algora::Vertex*> targetSet = {end};

    Algora::BreadthFirstSearch<Algora::FastPropertyMap,false> sourceBFS(false, false);
    sourceBFS.setStartVertex(start);
    sourceBFS.setArcStopCondition([&reachable, &targetSet](const Algora::Arc* arc){
        reachable = targetSet.count(arc->getHead());
        return reachable;
    });
    sourceBFS.setVertexStopCondition([&stepsTaken, &sourceSet, this](const Algora::Vertex* vertex){
        auto [it,inserted] = sourceSet.insert(vertex);
        stepsTaken += inserted;
        return stepsTaken == stepSize;
    });

    Algora::BreadthFirstSearch<Algora::FastPropertyMap, false> targetBFS(false, false);
    targetBFS.setStartVertex(end);
    targetBFS.reverseArcDirection(true);
    targetBFS.setArcStopCondition([&reachable, &sourceSet](const Algora::Arc* arc){
        reachable = sourceSet.count(arc->getTail());
        return reachable;
    });
    targetBFS.setVertexStopCondition([&stepsTaken,& targetSet, this](const Algora::Vertex* vertex){
        auto [it,inserted] = targetSet.insert(vertex);
        stepsTaken += inserted;
        return stepsTaken == stepSize;
    });

    runAlgorithm(sourceBFS, diGraph);
    if(reachable){
        return reachable;
    }
    else if(!stepsTaken){
        return false;
    }
    stepsTaken = 0ULL;

    runAlgorithm(targetBFS, diGraph);
    if(reachable){
        return reachable;
    }
    else if(!stepsTaken){
        return false;
    }
    stepsTaken = 0ULL;

    while(true){
        sourceBFS.resume();
        if(reachable){
            return reachable;
        }
        else if(!stepsTaken){
            return false;
        }
        stepsTaken = 0ULL;

        targetBFS.resume();
        if(reachable){
            return reachable;
        }
        else if(!stepsTaken){
            return false;
        }
        stepsTaken = 0ULL;
    }
}

std::string TwoWayBFSAPReachAlgorithm::getBaseName() {
    return std::__cxx11::string("2WayBFS(s=" + std::to_string(stepSize) + ")");
}

void TwoWayBFSAPReachAlgorithm::run() {
}

std::string TwoWayBFSAPReachAlgorithm::getName() const noexcept {
    return std::__cxx11::string("Two Way BFS All Pairs Algorithm");
}

std::string TwoWayBFSAPReachAlgorithm::getShortName() const noexcept {
    return std::__cxx11::string("2WayBFS AP");
}
