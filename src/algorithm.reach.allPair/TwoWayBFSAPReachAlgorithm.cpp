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
#include <algorithm.basic.traversal/breadthfirstsearch.h>
#include <property/fastpropertymap.h>
#include <unordered_set>
#include "TwoWayBFSAPReachAlgorithm.h"

bool TwoWayBFSAPReachAlgorithm::query(Algora::Vertex *start, const Algora::Vertex *end) {

    if(start == end){
        return true;
    }

    if(diGraph->isIsolated(start) || diGraph->isIsolated(end)){
        return false;
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
        if(!sourceReachable(vertex)){
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
        if(!targetReachable(vertex)){
            targetReachable[vertex] = true;
            stepsTaken++;
        }
        return stepsTaken == stepSize;
    });

    runAlgorithm(sourceBFS, diGraph);
    if(reachable){
        return reachable;
    }
    else if(stepsTaken != stepSize){
        return false;
    }
    stepsTaken = 0ULL;

    runAlgorithm(targetBFS, diGraph);
    if(reachable){
        return reachable;
    }
    else if(stepsTaken != stepSize){
        return false;
    }
    stepsTaken = 0ULL;

    while(true){
        sourceBFS.resume();
        if(reachable){
            return reachable;
        }
        else if(stepsTaken != stepSize){
            return false;
        }
        stepsTaken = 0ULL;

        targetBFS.resume();
        if(reachable){
            return reachable;
        }
        else if(stepsTaken != stepSize){
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
