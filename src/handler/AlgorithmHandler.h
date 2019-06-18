#include <utility>
#include <instanceprovider.h>
#include <property/fastpropertymap.h>

//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_ALGORITHMHANDLER_H
#define ALLPAIRREACH_ALGORITHMHANDLER_H


#include "graph.dyn/dynamicdigraph.h"

#include "../algorithm.reach.allPair/DynamicAPReachAlgorithm.h"
#include "../algorithm.reach.allPair/PartitionedDAPReachAlgorithmImplementation.h"


class AlgorithmHandler {

public:
    AlgorithmHandler(std::vector<PartitionedDAPReachAlgorithm*> newAlgorithms, Algora::InstanceProvider* newProvider) : algorithms(std::move(newAlgorithms)), instanceProvider{newProvider}{}

    void runInterface();

    void runTests(unsigned long long k);

private:
    void addArc();

    void removeArc();

private:

    struct TimeCollector;

    std::vector<PartitionedDAPReachAlgorithm*> algorithms;

    Algora::InstanceProvider* instanceProvider;

    void reachabilityCheck();

    void writeResults(const std::vector<TimeCollector*>& timers);
    
};


#endif //ALLPAIRREACH_ALGORITHMHANDLER_H
