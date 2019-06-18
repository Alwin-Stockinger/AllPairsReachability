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
    explicit AlgorithmHandler(Algora::InstanceProvider* newProvider) : instanceProvider{newProvider}{}

    void runInterface();

    void runTests(unsigned long long k,  const std::vector<std::string>& algorithmNames);

private:
    void addArc();

    void removeArc();

private:

    struct TimeCollector;

    Algora::InstanceProvider* instanceProvider;

    void reachabilityCheck();

    void writeResults(const std::vector<TimeCollector*>& timers);

    std::vector<PartitionedDAPReachAlgorithm*>* createAlgorithms(const std::vector<std::string>& algorithmNames);

    template<typename T>
    PartitionedDAPReachAlgorithm  *createAlgorithm();
};


#endif //ALLPAIRREACH_ALGORITHMHANDLER_H
