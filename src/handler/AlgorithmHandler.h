#include <utility>
#include <instanceprovider.h>

//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_ALGORITHMHANDLER_H
#define ALLPAIRREACH_ALGORITHMHANDLER_H


#include "graph.dyn/dynamicdigraph.h"

#include "../algorithm.reach.allPair/DynamicAPReachAlgorithm.h"



class AlgorithmHandler {

public:
    AlgorithmHandler(std::vector<DynamicAPReachAlgorithm *> newAlgorithms, Algora::InstanceProvider* newProvider) : algorithms(std::move(newAlgorithms)), instanceProvider{newProvider}{}

    void runInterface();

    void runTests();

private:
    void addArc();

    void removeArc();

private:

    struct TimeCollector;

    std::vector<DynamicAPReachAlgorithm *> algorithms;

    Algora::InstanceProvider* instanceProvider;

    void reachabilityCheck();

    void writeResults(const std::vector<TimeCollector*>& timers);


};


#endif //ALLPAIRREACH_ALGORITHMHANDLER_H
