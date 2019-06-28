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

    void runTests(unsigned long long const kMax, const std::vector<std::string> &algorithmNames,
                  const bool detailedResults);

private:
    void addArc();

    void removeArc();

private:

    struct TimeCollector;

    Algora::InstanceProvider* instanceProvider;

    void reachabilityCheck();

    static void writeResults(TimeCollector& timer);
    void writeHeader();

    std::vector<DynamicAPReachAlgorithm *> *
    createPartitionedAlgorithms(const std::vector<std::string> &algorithmNames, unsigned long long int k,
                                const Algora::FastPropertyMap<unsigned long long int>& partition);

    template<typename T, unsigned Level = 0U>
    PartitionedDAPReachAlgorithm *createPartitionAlgorithm();

    void writeDetailedResults(const TimeCollector& collector);

    void writeDetailedHeader();
};


#endif //ALLPAIRREACH_ALGORITHMHANDLER_H
