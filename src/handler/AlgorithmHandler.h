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

    void runTests(unsigned long long kMax,  const std::vector<std::string>& algorithmNames);

private:
    void addArc();

    void removeArc();

private:

    struct TimeCollector;

    Algora::InstanceProvider* instanceProvider;

    void reachabilityCheck();

    void writeAllResults(const std::vector<TimeCollector*>& timers);

    void writeResults(TimeCollector& timer);
    void writeHeader();

    std::vector<DynamicAPReachAlgorithm*>* createAlgorithms(const std::vector<std::string>& algorithmNames, unsigned long long i, std::vector<unsigned> levels);

    template<typename T, unsigned Level = 0U>
    DynamicAPReachAlgorithm  *createAlgorithm(unsigned long long k);
};


#endif //ALLPAIRREACH_ALGORITHMHANDLER_H
