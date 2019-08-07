//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_FACTORYFACADE_H
#define ALLPAIRREACH_FACTORYFACADE_H


#include "factories/SimplePartitionFactory.h"
#include "factories/SuperVertexFactory.h"
#include "factories/SSBasedFactory.h"
#include "factories/BFSOFactory.h"
#include "factories/ReverseBFSFactory.h"


using PartFunc = std::function<Algora::FastPropertyMap<unsigned long long>(unsigned long long int, Algora::DiGraph*)>;

class FactoryFacade {

public:

    FactoryFacade(bool ssBased, bool simplePartition, bool bFSO, bool superVertex, bool reverseBFS){
        if(ssBased){
            factories.push_back(&ssBasedFactory);
        }
        if(simplePartition){
            factories.push_back(&simplePartitionFactory);
            partitionedFactories.push_back(&simplePartitionFactory);
        }
        if(bFSO){
            factories.push_back(&bfsoFactory);
            partitionedFactories.push_back(&bfsoFactory);
        }
        if(superVertex){
            factories.push_back(&superVertexFactory);
            partitionedFactories.push_back(&superVertexFactory);
        }
        if(reverseBFS){
            factories.push_back(&reverseBFSFactory);
            partitionedFactories.push_back(&reverseBFSFactory);
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

    void setPartitionFunction(const PartFunc& partFunc){
        for(PartitionedFactory* partitionedFactory: partitionedFactories){
            partitionedFactory->setPartitionFunction(partFunc);
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

    void setAdvancedBFSO(bool b) {
        bfsoFactory.setAdvancedBFSO(b);
        reverseBFSFactory.setAdvancedBFSO(b);
    }

    void setSimpleBFSO(bool b){
        bfsoFactory.setSimpleBFSO(b);
        reverseBFSFactory.setSimpleBFSO(b);
    }

private:
    BFSOFactory bfsoFactory{};
    SimplePartitionFactory simplePartitionFactory{};
    SuperVertexFactory superVertexFactory{};
    SSBasedFactory ssBasedFactory{};
    ReverseBFSFactory reverseBFSFactory{};

    std::vector<AlgorithmFactory*> factories{};
    std::vector<PartitionedFactory*> partitionedFactories{};
};


#endif //ALLPAIRREACH_FACTORYFACADE_H
