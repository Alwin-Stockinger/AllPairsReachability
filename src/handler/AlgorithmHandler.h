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

    void runTests(const std::vector<std::string> &algorithmNames, const unsigned long long int kMax,
                  const unsigned long long int kMin, const unsigned long long int timeOut,
                  const bool detailedResults, const unsigned int minLevel, const unsigned int maxLevel,
                  const bool withoutPartition);

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
    createPartitionedAlgorithms(const std::vector<std::string> &algorithmNames,
                                const unsigned long long int k,
                                std::map<std::string, Algora::FastPropertyMap<unsigned long long int>> &partitions,
                                const unsigned depth);

    template<typename T>
    PartitionedDAPReachAlgorithm *createPartitionAlgorithm(unsigned depth = 0U);

    static void writeDetailedResults(const TimeCollector& collector);

    void writeDetailedHeader();

    void runTest(DynamicAPReachAlgorithm *algorithm, TimeCollector &timer, const unsigned long long &timeOut);
};


#endif //ALLPAIRREACH_ALGORITHMHANDLER_H
