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

    Algora::FastPropertyMap<bool> sourceReachable(false);
    sourceReachable[start] = true;

    Algora::FastPropertyMap<bool> targetReachable(false);
    targetReachable[end] = true;

    //std::unordered_set<const Algora::Vertex*> sourceSet = {start};
    //std::unordered_set<const Algora::Vertex*> targetSet = {end};

    Algora::BreadthFirstSearch<Algora::FastPropertyMap,false> sourceBFS(false, false);
    sourceBFS.setStartVertex(start);
    sourceBFS.setArcStopCondition([&reachable, &targetReachable](const Algora::Arc* arc){
        reachable = targetReachable(arc->getHead());
        return reachable;
    });
    sourceBFS.setVertexStopCondition([&stepsTaken, &sourceReachable, this](const Algora::Vertex* vertex){
        bool alreadyFound = sourceReachable(vertex);
        if(!alreadyFound){
            sourceReachable[vertex] = true;
            stepsTaken++;
        }
        return stepsTaken == stepSize;
    });



    Algora::BreadthFirstSearch<Algora::FastPropertyMap, false> targetBFS(false, false);
    targetBFS.setStartVertex(end);
    targetBFS.reverseArcDirection(true);
    targetBFS.setArcStopCondition([&reachable, &sourceReachable](const Algora::Arc* arc){
        reachable = sourceReachable(arc->getTail());
        return reachable;
    });
    targetBFS.setVertexStopCondition([&stepsTaken,&targetReachable, this](const Algora::Vertex* vertex){
        bool alreadyFound = targetReachable(vertex);
        if(!alreadyFound){
            targetReachable[vertex] = true;
            stepsTaken++;
        }
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
