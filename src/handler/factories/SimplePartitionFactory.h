//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_SIMPLEPARTITIONFACTORY_H
#define ALLPAIRREACH_SIMPLEPARTITIONFACTORY_H


#include "PartitionedFactory.h"
#include "../../algorithm.reach.allPair/PartitionedAPReachAlgorithm.h"

class SimplePartitionFactory : public PartitionedFactory{

    std::vector<DynamicAPReachAlgorithm *> getStaticBFSAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getStaticDFSAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getCachingBFSAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getCachingDFSAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getLazyBFSAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getLazyDFSAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getSimpleISSRAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getSimpleESTAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getMLESTAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getESTAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getOldESTAlgorithms() override;




private:

    template <typename SubAlgorithm>
    std::vector<DynamicAPReachAlgorithm*> createAlgorithms();

    template <typename SubAlgorithm, typename OverlayAlgorithm>
    std::vector<DynamicAPReachAlgorithm*> createSpecificAlgorithms();

};


#endif //ALLPAIRREACH_SIMPLEPARTITIONFACTORY_H
