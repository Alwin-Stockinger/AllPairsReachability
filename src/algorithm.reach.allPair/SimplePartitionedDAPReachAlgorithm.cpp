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
