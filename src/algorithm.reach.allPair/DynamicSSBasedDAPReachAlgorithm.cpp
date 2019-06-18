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


bool DynamicSSBasedDAPReachAlgorithm::query(Algora::Vertex *start, const Algora::Vertex *end) {
    return vertexMap[start]->query(end);
}


void DynamicSSBasedDAPReachAlgorithm::run() {
    for (auto &&algorithm : vertexMap) {
        algorithm->run();
    }
}


void DynamicSSBasedDAPReachAlgorithm::onVertexAdd(Algora::Vertex *vertex) {
    DynamicDiGraphAlgorithm::onVertexAdd(vertex);

    addVertexToMap(vertex);
}


void DynamicSSBasedDAPReachAlgorithm::onVertexRemove(Algora::Vertex *vertex) {
    DynamicDiGraphAlgorithm::onVertexRemove(vertex);

    delete vertexMap[vertex];
    vertexMap.resetToDefault(vertex);
}


void DynamicSSBasedDAPReachAlgorithm::init() {
    diGraph->mapVertices([this](Algora::Vertex *vertex) {
        addVertexToMap(vertex);
    });
}


void DynamicSSBasedDAPReachAlgorithm::addVertexToMap(Algora::Vertex *vertex) {

    Algora::DynamicSSReachAlgorithm *algorithm = createSSAlgorithm();

    algorithm->setGraph(diGraph);
    algorithm->setSource(vertex);

    vertexMap[vertex] = algorithm;
}


std::string DynamicSSBasedDAPReachAlgorithm::getName() const noexcept {
    return std::__cxx11::string("Single Source based All Pair Reachability based on " + vertexMap.getValue(diGraph->getAnyVertex()) -> getName());
}


std::string DynamicSSBasedDAPReachAlgorithm::getShortName() const noexcept {
    return std::__cxx11::string("SS AP based on " + vertexMap.getValue(diGraph->getAnyVertex()) -> getShortName());
}


void DynamicSSBasedDAPReachAlgorithm::reset() {
    deleteAllAlgorithms();
    vertexMap.resetAll();
}


DynamicSSBasedDAPReachAlgorithm::~DynamicSSBasedDAPReachAlgorithm() {
    deleteAllAlgorithms();
}


void DynamicSSBasedDAPReachAlgorithm::deleteAllAlgorithms(){
    for (auto &&algorithm : vertexMap) {
        delete algorithm;
    }
}