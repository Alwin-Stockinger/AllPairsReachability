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

#include "SSBasedDAPReachAlgorithm.h"

template<typename T>
bool SSBasedDAPReachAlgorithm<T>::query(const Algora::Vertex *start, const Algora::Vertex *end) {
    return vertexMap[start]->query(end);
}

template<typename T>
void SSBasedDAPReachAlgorithm<T>::run() {
    for (auto &&algorithm : vertexMap) {
        algorithm->run();
    }
}

template<typename T>
void SSBasedDAPReachAlgorithm<T>::onVertexAdd(Algora::Vertex *vertex) {
    DynamicDiGraphAlgorithm::onVertexAdd(vertex);

    addVertexToMap(vertex);
}

template<typename T>
void SSBasedDAPReachAlgorithm<T>::onVertexRemove(Algora::Vertex *vertex) {
    DynamicDiGraphAlgorithm::onVertexRemove(vertex);

    delete vertexMap[vertex];
    vertexMap.resetToDefault(vertex);
}

template<typename T>
void SSBasedDAPReachAlgorithm<T>::init() {
    diGraph->mapVertices([this](Algora::Vertex *vertex) {
        addVertexToMap(vertex);
    });
}

template<typename T>
void SSBasedDAPReachAlgorithm<T>::addVertexToMap(Algora::Vertex *vertex) {

    auto *algorithm = new T;

    algorithm->setGraph(diGraph);
    algorithm->setSource(vertex);

    vertexMap[vertex] = algorithm;
}

template<typename T>
std::string SSBasedDAPReachAlgorithm<T>::getName() const noexcept {
    return std::__cxx11::string("All Pair Reachability based on Single Source Reachability Algorithm");
}

template<typename T>
std::string SSBasedDAPReachAlgorithm<T>::getShortName() const noexcept {
    return std::__cxx11::string("AP Reach SS based");
}

template<typename T>
void SSBasedDAPReachAlgorithm<T>::reset() {

    deleteAllAlgorithms();

    vertexMap.resetAll();
    if(diGraph) init();
}

template<typename T>
SSBasedDAPReachAlgorithm<T>::~SSBasedDAPReachAlgorithm() {
    deleteAllAlgorithms();
}

template<typename T>
void SSBasedDAPReachAlgorithm<T>::deleteAllAlgorithms(){
    for (auto &&algorithm : vertexMap) {
        delete algorithm;
    }
}




template class SSBasedDAPReachAlgorithm<Algora::StaticBFSSSReachAlgorithm>;
template class SSBasedDAPReachAlgorithm<Algora::StaticDFSSSReachAlgorithm>;
template class SSBasedDAPReachAlgorithm<Algora::LazyBFSSSReachAlgorithm>;
template class SSBasedDAPReachAlgorithm<Algora::LazyDFSSSReachAlgorithm>;
template class SSBasedDAPReachAlgorithm<Algora::CachingDFSSSReachAlgorithm>;
template class SSBasedDAPReachAlgorithm<Algora::CachingBFSSSReachAlgorithm>;
template class SSBasedDAPReachAlgorithm<Algora::SimpleIncSSReachAlgorithm>;
template class SSBasedDAPReachAlgorithm<Algora::ESTreeML>;
template class SSBasedDAPReachAlgorithm<Algora::OldESTree>;
template class SSBasedDAPReachAlgorithm<Algora::ESTreeQ>;
template class SSBasedDAPReachAlgorithm<Algora::SimpleESTree>;