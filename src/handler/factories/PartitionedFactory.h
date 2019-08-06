//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_PARTITIONEDFACTORY_H
#define ALLPAIRREACH_PARTITIONEDFACTORY_H


#include <property/fastpropertymap.h>
#include "AlgorithmFactory.h"
#include "../../partition/Partitioner.h"
#include "../../algorithm.reach.allPair/PartitionedDAPReachAlgorithm.h"

using PartFunc = std::function<Algora::FastPropertyMap<unsigned long long>(unsigned long long int, Algora::DiGraph*)>;

class PartitionedFactory : public AlgorithmFactory{

public:
    void setOverlayNames(const std::vector<std::string> &overlayNames);

    void setKMin(unsigned long long int kMin);

    void setKMax(unsigned long long int kMax);

    void setDepthMin(unsigned long long int depthMin);

    void setDepthMax(unsigned long long int depthMax);

    void setPartitionFunction(const PartFunc &partitionFunction);

    void setExponentialK(bool exponentialK);

    void setRepartitionThreshold(unsigned long long int threshold);

protected:

    void configureAlgorithm(PartitionedDAPReachAlgorithm* algorithm){
        algorithm->setPartitionFunction(partitionFunction);
        algorithm->setRepartitionThreshold(repartitionThreshold);
    }

    PartFunc partitionFunction = Partitioner::handlePartitioning;
    std::vector<std::string> overlayNames;

    unsigned long long kMin = 2;
    unsigned long long kMax = 2;
    bool exponentialK = true;

    unsigned long long depthMin = 0;
    unsigned long long depthMax = 0;

    unsigned long long repartitionThreshold = 0ULL;
};


#endif //ALLPAIRREACH_PARTITIONEDFACTORY_H
