//
// Created by Alwin Stockinger.
//

#include <iostream>
#include "DynamicAPReachAlgorithm.h"

bool DynamicAPReachAlgorithm::containsVertex(const Algora::Vertex *vertex) {
    return diGraph->containsVertex(vertex);
}

Algora::Arc* DynamicAPReachAlgorithm::addArc(Algora::Vertex *head, Algora::Vertex *tail) {
    return diGraph->addArc(tail, head);
}

void DynamicAPReachAlgorithm::removeArc(Algora::Vertex *head, Algora::Vertex *tail) {
    Algora::Arc* arc = diGraph->findArc(head, tail);
    diGraph->removeArc(arc);
}

bool DynamicAPReachAlgorithm::isIsolated(const Algora::Vertex* vertex) {
    return diGraph->isIsolated(vertex);
}
