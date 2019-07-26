//
// Created by Alwin Stockinger.
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
    for (auto &&algorithm : vertexMap) {
        algorithm->run();
    }

    initialized = true;
}

template<bool immediateInit>
void DynamicSSBasedDAPReachAlgorithm<immediateInit>::onVertexAdd(Algora::Vertex *vertex) {
    if(initialized){
        DynamicDiGraphAlgorithm::onVertexAdd(vertex);

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

    if(initialized){
        DynamicDiGraphAlgorithm::onVertexRemove(vertex);
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

    if(initialized) {
        DynamicDiGraphAlgorithm::onArcAdd(arc);
        for(auto* algorithm: vertexMap){
            if(algorithm){
                algorithm->onArcAdd(arc);
            }
        }
    }
}

template<bool immediateInit>
void DynamicSSBasedDAPReachAlgorithm<immediateInit>::onArcRemove(Algora::Arc *arc) {
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

    //important for testing, otherwise major work happens outside meassurement
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
    for (auto &&algorithm : vertexMap) {
        delete algorithm;
    }
}

template class DynamicSSBasedDAPReachAlgorithm<false>;
template class DynamicSSBasedDAPReachAlgorithm<true>;