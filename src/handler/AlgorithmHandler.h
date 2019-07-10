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

    void runTests(const std::vector<std::string> &algorithmNames, bool exponentialK,
                  unsigned long long int kMax, unsigned long long int kMin,
                  unsigned long long int timeOut, bool detailedResults,
                  unsigned int minLevel, unsigned int maxLevel, bool withoutPartition);

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
                                unsigned long long int k,
                                const Algora::FastPropertyMap<unsigned long long int>& partitions,
                                unsigned depth);

    template<typename SubAlgorithm, typename OverlayAlgorithm = SubAlgorithm>
    PartitionedDAPReachAlgorithm *createPartitionAlgorithm(unsigned depth = 0U);

    std::vector<DynamicAPReachAlgorithm *> * createSpecialOverlayPartitionedAlgorithms(std::vector<std::string> &algorithmNames,
                                                                                       std::vector<std::string> &overlayNames,
                                                                                        unsigned long long int k,
                                                                                        const Algora::FastPropertyMap<unsigned long long int>& partitions,
                                                                                        unsigned depth);

    template<typename OverlayAlgorithm>
    std::vector<DynamicAPReachAlgorithm*> createPartitionedAlgorithmForOverlay(
            const std::vector<std::string> &algorithmNames,
            unsigned long long int k,
            const Algora::FastPropertyMap<unsigned long long int>& partitions,
            unsigned depth = 0U);

    static void writeDetailedResults(const TimeCollector& collector);

    void writeDetailedHeader();

    void runTest(DynamicAPReachAlgorithm *algorithm, TimeCollector &timer, const unsigned long long &timeOut);

    static std::vector<DynamicAPReachAlgorithm *>* createAPAlgorithms(const std::vector<std::string> &algorithmNames);
};


#endif //ALLPAIRREACH_ALGORITHMHANDLER_H
