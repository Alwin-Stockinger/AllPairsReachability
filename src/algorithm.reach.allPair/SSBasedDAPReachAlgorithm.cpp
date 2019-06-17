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


bool SSBasedDAPReachAlgorithm::query(const Algora::Vertex *start, const Algora::Vertex *end) {
    return vertexMap[start]->query(end);
}


void SSBasedDAPReachAlgorithm::run() {
    for (auto &&algorithm : vertexMap) {
        algorithm->run();
    }
}


void SSBasedDAPReachAlgorithm::onVertexAdd(Algora::Vertex *vertex) {
    DynamicDiGraphAlgorithm::onVertexAdd(vertex);

    addVertexToMap(vertex);
}


void SSBasedDAPReachAlgorithm::onVertexRemove(Algora::Vertex *vertex) {
    DynamicDiGraphAlgorithm::onVertexRemove(vertex);

    delete vertexMap[vertex];
    vertexMap.resetToDefault(vertex);
}


void SSBasedDAPReachAlgorithm::init() {
    diGraph->mapVertices([this](Algora::Vertex *vertex) {
        addVertexToMap(vertex);
    });
}


void SSBasedDAPReachAlgorithm::addVertexToMap(Algora::Vertex *vertex) {

    Algora::DynamicSSReachAlgorithm *algorithm = createSSAlgorithm();

    algorithm->setGraph(diGraph);
    algorithm->setSource(vertex);

    vertexMap[vertex] = algorithm;
}


std::string SSBasedDAPReachAlgorithm::getName() const noexcept {
    return std::__cxx11::string("Single Source based All Pair Reachability based on " + vertexMap.getValue(diGraph->getAnyVertex()) -> getName());
}


std::string SSBasedDAPReachAlgorithm::getShortName() const noexcept {
    return std::__cxx11::string("SS AP based on " + vertexMap.getValue(diGraph->getAnyVertex()) -> getName());
}


void SSBasedDAPReachAlgorithm::reset() {
    deleteAllAlgorithms();
    vertexMap.resetAll();
}


SSBasedDAPReachAlgorithm::~SSBasedDAPReachAlgorithm() {
    deleteAllAlgorithms();
}


void SSBasedDAPReachAlgorithm::deleteAllAlgorithms(){
    for (auto &&algorithm : vertexMap) {
        delete algorithm;
    }
}