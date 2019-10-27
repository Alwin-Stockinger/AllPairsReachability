//
// Created by Alwin Stockinger.
//

#include "StaticSSBasedDAPReachAlgorithm.h"

void StaticSSBasedDAPReachAlgorithm::run() {
    algorithm->run();
}

std::string StaticSSBasedDAPReachAlgorithm::getName() const noexcept {
    return std::__cxx11::string("Single Source based All Pair Reachability based on " + algorithm->getName());
}

std::string StaticSSBasedDAPReachAlgorithm::getShortName() const noexcept {
    return std::__cxx11::string("SS AP based on " + algorithm->getShortName());
}

void StaticSSBasedDAPReachAlgorithm::onDiGraphSet() {
    DynamicDiGraphAlgorithm::onDiGraphSet();
    if(!algorithm){
        algorithm = createSSAlgorithm();
    }
    algorithm->setGraph(diGraph);
}

void StaticSSBasedDAPReachAlgorithm::onDiGraphUnset() {
    if(algorithm) algorithm->unsetGraph();
    DynamicDiGraphAlgorithm::onDiGraphUnset();
}

StaticSSBasedDAPReachAlgorithm::StaticSSBasedDAPReachAlgorithm() : SSBasedDAPReachAlgorithm() {}

StaticSSBasedDAPReachAlgorithm::~StaticSSBasedDAPReachAlgorithm() {
    delete algorithm;
}

bool StaticSSBasedDAPReachAlgorithm::query(Algora::Vertex *start, const Algora::Vertex *end) {
    if(start == end){
        return true;
    }
    else if(diGraph->isIsolated(end)){
        return false;
    }
    algorithm->setSource(start);
    return algorithm->query(end);
}

std::string StaticSSBasedDAPReachAlgorithm::getBaseName() {
    return algorithm->getShortName();
}
