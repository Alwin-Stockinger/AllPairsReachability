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

//needed because of template
#include <algorithm.reach/staticbfsssreachalgorithm.h>
#include <algorithm.reach/staticdfsssreachalgorithm.h>
#include <algorithm.reach/cachingbfsssreachalgorithm.h>
#include <algorithm.reach/cachingdfsssreachalgorithm.h>
#include <algorithm.reach/lazydfsssreachalgorithm.h>
#include <algorithm.reach/lazybfsssreachalgorithm.h>
#include <algorithm.reach/simpleincssreachalgorithm.h>
#include <algorithm.reach.es/estree-ml.h>
#include <algorithm.reach.es/estree-bqueue.h>
#include <algorithm.reach.es/estree-queue.h>
#include <algorithm.reach.es/simpleestree.h>

#include "DynamicSSBasedDAPReachAlgorithm.h"


template<bool immediateInit>
bool DynamicSSBasedDAPReachAlgorithm<immediateInit>::query(Algora::Vertex *start, const Algora::Vertex *end) {

    if(start == end){
        return true;
    }
    else if(diGraph->isIsolated(end)){
        return false;
    }

    if(!initialized){
        run();
    }

    return vertexMap[start]->query(end);
}

template<bool immediateInit>
void DynamicSSBasedDAPReachAlgorithm<immediateInit>::run() {
    if(!initialized){
        init();
    }
    if(immediateInit){
        for (auto* algorithm : vertexMap) {
            if(algorithm){
                algorithm->run();
            }
        }
    }

    initialized = true;
}

template<bool immediateInit>
void DynamicSSBasedDAPReachAlgorithm<immediateInit>::onVertexAdd(Algora::Vertex *vertex) {

    DynamicDiGraphAlgorithm::onVertexAdd(vertex);

    if(initialized){
        for(auto* algorithm: vertexMap){
            if(algorithm){
                algorithm->onVertexAdd(vertex);
            }
        }

        addVertexToMap(vertex);
        if(immediateInit){
            vertexMap[vertex]->run();
        }

    }
}

template<bool immediateInit>
void DynamicSSBasedDAPReachAlgorithm<immediateInit>::onVertexRemove(Algora::Vertex *vertex) {
    DynamicDiGraphAlgorithm::onVertexRemove(vertex);
    if(initialized){
        delete vertexMap[vertex];
        vertexMap.resetToDefault(vertex);

        for(auto* algorithm: vertexMap){
            if(algorithm){
                algorithm->onVertexRemove(vertex);
            }
        }
    }
}

template<bool immediateInit>
void DynamicSSBasedDAPReachAlgorithm<immediateInit>::onArcAdd(Algora::Arc *arc) {
    DynamicDiGraphAlgorithm::onArcAdd(arc);
    if(initialized) {
        for(auto* algorithm: vertexMap){
            if(algorithm){
                algorithm->onArcAdd(arc);
            }
        }
    }
}

template<bool immediateInit>
void DynamicSSBasedDAPReachAlgorithm<immediateInit>::onArcRemove(Algora::Arc *arc) {
    DynamicDiGraphAlgorithm::onArcRemove(arc);
    if(initialized){
        for(auto* algorithm: vertexMap){
            if(algorithm){
                algorithm->onArcRemove(arc);
            }
        }
    }
}

template<bool immediateInit>
void DynamicSSBasedDAPReachAlgorithm<immediateInit>::init() {
    diGraph->mapVertices([this](Algora::Vertex *vertex) {
        addVertexToMap(vertex);
    });
}

template<bool immediateInit>
void DynamicSSBasedDAPReachAlgorithm<immediateInit>::addVertexToMap(Algora::Vertex *vertex) {

    Algora::DynamicSSReachAlgorithm *algorithm = createSSAlgorithm();

    //important for testing, otherwise major work happens outside measurement when only testing SSBased
    algorithm->setAutoUpdate(false);

    algorithm->setGraph(diGraph);
    algorithm->setSource(vertex);

    vertexMap[vertex] = algorithm;
}

template<bool immediateInit>
std::string DynamicSSBasedDAPReachAlgorithm<immediateInit>::getName() const noexcept {
    return std::__cxx11::string("Single Source based All Pair Reachability based on " + vertexMap.getValue(diGraph->getAnyVertex()) -> getName());
}

template<bool immediateInit>
std::string DynamicSSBasedDAPReachAlgorithm<immediateInit>::getShortName() const noexcept {
    return std::__cxx11::string("SS AP based on " + vertexMap.getValue(diGraph->getAnyVertex()) -> getShortName());
}

template<bool immediateInit>
void DynamicSSBasedDAPReachAlgorithm<immediateInit>::reset() {
    deleteAllAlgorithms();
    vertexMap.resetAll();
    initialized = false;
}

template<bool immediateInit>
void DynamicSSBasedDAPReachAlgorithm<immediateInit>::deleteAllAlgorithms(){
    for (auto* algorithm : vertexMap) {
        delete algorithm;
    }
}

template class DynamicSSBasedDAPReachAlgorithm<false>;
template class DynamicSSBasedDAPReachAlgorithm<true>;