//
// Created by Alwin Stockinger.
//

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

    vertexMap.resetToDefault(vertex);
}

void SSBasedDAPReachAlgorithm::init() {
    diGraph->mapVertices([this](Algora::Vertex *vertex) {
        addVertexToMap(vertex);
    });
}

void SSBasedDAPReachAlgorithm::addVertexToMap(Algora::Vertex *vertex) {
    auto *algorithm = createAlgorithm();

    algorithm->setGraph(
            diGraph);   //bug probably happens because algorithm registers als listener and then it messes with the array bounds of the listener array, first doesnt get Seg fault because array messing happens in an unobtrusive way?
    algorithm->setSource(vertex);

    vertexMap[vertex] = std::unique_ptr<SSRAlgo>(algorithm);
}