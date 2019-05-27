//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_ALGORITHMHANDLER_H
#define ALLPAIRREACH_ALGORITHMHANDLER_H


#include "graph.dyn/dynamicdigraph.h"

#include "../algorithm.reach.allPair/DynamicAPReachAlgorithm.h"

class AlgorithmHandler {

public:
    AlgorithmHandler(DynamicAPReachAlgorithm *newAlgorithm, Algora::DynamicDiGraph *newGraph) : algorithm(newAlgorithm), graph(newGraph){}

    void run();

private:
    void addArc();

    void removeArc();

private:
    DynamicAPReachAlgorithm *algorithm;

    Algora::DynamicDiGraph *graph;

    void reachabilityCheck();
};


#endif //ALLPAIRREACH_ALGORITHMHANDLER_H
