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
#include "factories/DFSOFactory.h"
#include "factories/TwoWayFactory.h"
#include "factories/BiSuperFactory.h"


using PartFunc = std::function<Algora::FastPropertyMap<unsigned long long>(unsigned long long int, Algora::DiGraph*)>;

class FactoryFacade {

public:

    FactoryFacade(bool ssBased, bool simplePartition, bool bFSO, bool superVertex, bool reverseBFS, bool dFSO, bool twoWay, bool biSuper){
        if(ssBased){
            factories.push_back(&ssBasedFactory);
        }
        if(twoWay){
            factories.push_back(&twoWayFactory);
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
        if(dFSO){
            factories.push_back(&dfsoFactory);
            partitionedFactories.push_back(&dfsoFactory);
        }
        if(biSuper){
            factories.push_back(&biSuperFactory);
            partitionedFactories.push_back(&biSuperFactory);
        }
    }

    void setMinTwoWaySteps(unsigned long long stepSize){
        twoWayFactory.setMinSteps(stepSize);
        biSuperFactory.setMinSteps(stepSize);
    }
    void setMaxTwoWaySteps(unsigned long long stepSize){
        twoWayFactory.setMaxSteps(stepSize);
        biSuperFactory.setMaxSteps(stepSize);
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

    void setRandomVertexAdditions(bool b){
        for(PartitionedFactory* factory: partitionedFactories){
            factory->setRandomVertexAdditions(b);
        }
    }


    void setRepartitionThreshold(unsigned long long int threshold) {
        for(PartitionedFactory* factory: partitionedFactories){
            factory->setRepartitionThreshold(threshold);
        }

    }

    void setAdvancedBFSO(bool b) {
        bfsoFactory.setAdvancedBFSO(b);
        dfsoFactory.setAdvancedDFSO(b);
        reverseBFSFactory.setAdvancedBFSO(b);
    }

    void setSimpleBFSO(bool b){
        bfsoFactory.setSimpleBFSO(b);
        dfsoFactory.setSimpleDFSO(b);
        reverseBFSFactory.setSimpleBFSO(b);
    }

private:
    BFSOFactory bfsoFactory{};
    DFSOFactory dfsoFactory{};
    SimplePartitionFactory simplePartitionFactory{};
    SuperVertexFactory superVertexFactory{};
    SSBasedFactory ssBasedFactory{};
    ReverseBFSFactory reverseBFSFactory{};
    TwoWayFactory twoWayFactory{};
    BiSuperFactory biSuperFactory{};


    std::vector<AlgorithmFactory*> factories{};
    std::vector<PartitionedFactory*> partitionedFactories{};
};


#endif //ALLPAIRREACH_FACTORYFACADE_H
