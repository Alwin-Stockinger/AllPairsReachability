#include <utility>

//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_ALGORITHMHANDLER_H
#define ALLPAIRREACH_ALGORITHMHANDLER_H


#include "graph.dyn/dynamicdigraph.h"

#include "../algorithm.reach.allPair/DynamicAPReachAlgorithm.h"

class AlgorithmHandler {

public:
    AlgorithmHandler(std::vector<DynamicAPReachAlgorithm *> newAlgorithms, Algora::DynamicDiGraph *newGraph) : algorithms(std::move(newAlgorithms)), graph(newGraph){}

    void runInterface();

    void runTests();

private:
    void addArc();

    void removeArc();

private:
    std::vector<DynamicAPReachAlgorithm *> algorithms;

    Algora::DynamicDiGraph *graph;

    void reachabilityCheck();
};


#endif //ALLPAIRREACH_ALGORITHMHANDLER_H
