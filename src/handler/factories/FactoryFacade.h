//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_FACTORYFACADE_H
#define ALLPAIRREACH_FACTORYFACADE_H


#include "SimplePartitionFactory.h"
#include "SuperVertexFactory.h"
#include "SSBasedFactory.h"
#include "ABFSOFactory.h"
#include "SBFSOFactory.h"

class FactoryFacade {

public:

    FactoryFacade(bool ssBased, bool simplePartition, bool aBFSO, bool superVertex, bool simpleBFSO){
        if(ssBased){
            factories.push_back(&ssBasedFactory);
        }
        if(simplePartition){
            factories.push_back(&simplePartitionFactory);
            partitionedFactories.push_back(&simplePartitionFactory);
        }
        if(aBFSO){
            factories.push_back(&abfsoFactory);
            partitionedFactories.push_back(&abfsoFactory);
        }
        if(superVertex){
            factories.push_back(&superVertexFactory);
            partitionedFactories.push_back(&superVertexFactory);
        }
        if(simpleBFSO){
            factories.push_back(&sbfsoFactory);
            partitionedFactories.push_back(&sbfsoFactory);
        }
    }

    void setExponentialK(bool expoK){
        for(PartitionedFactory* partitionedFactory: partitionedFactories){
            partitionedFactory->setExponentialK(expoK);
        }
    }

    void setKMin(unsigned long long kMin){
        for(PartitionedFactory* partitionedFactory: partitionedFactories){
            partitionedFactory->setKMin(kMin);
        }
    }

    void setKMax( unsigned long long kMax){
        for(PartitionedFactory* partitionedFactory: partitionedFactories){
            partitionedFactory->setKMax(kMax);
        }
    }

    void setOverlayNames(const std::vector<std::string>& overlaynames){
        for(PartitionedFactory* partitionedFactory: partitionedFactories){
            partitionedFactory->setOverlayNames(overlaynames);
        }
    }

    void setMinDepth(const unsigned long long minDepth){
        for(PartitionedFactory *partitionedFactory: partitionedFactories){
            partitionedFactory->setDepthMin(minDepth);
        }
    }

    void setMaxDepth(const unsigned long long maxDepth){
        for(PartitionedFactory* partitionedFactory: partitionedFactories){
            partitionedFactory->setDepthMax(maxDepth);
        }
    }

    void setAlgorithmNames(const std::vector<std::string>& algorithmNames){
        for(AlgorithmFactory* factory: factories){
            factory->setAlgorithmNames(algorithmNames);
        }
    }

    std::vector<DynamicAPReachAlgorithm*> getAlgorithms(){

        std::vector<DynamicAPReachAlgorithm*> algorithms;

        for(AlgorithmFactory* factory: factories){
            auto factoryAlgorithms = factory->getAlgorithms();

            algorithms.insert(algorithms.end(), factoryAlgorithms.begin(), factoryAlgorithms.end());
        }

        return algorithms;
    }


    void setRepartitionThreshold(unsigned long long int threshold) {
        for(PartitionedFactory* factory: partitionedFactories){
            factory->setRepartitionThreshold(threshold);
        }

    }

private:
    ABFSOFactory abfsoFactory{};
    SBFSOFactory sbfsoFactory{};
    SimplePartitionFactory simplePartitionFactory{};
    SuperVertexFactory superVertexFactory{};
    SSBasedFactory ssBasedFactory{};

    std::vector<AlgorithmFactory*> factories{};
    std::vector<PartitionedFactory*> partitionedFactories{};
};


#endif //ALLPAIRREACH_FACTORYFACADE_H
