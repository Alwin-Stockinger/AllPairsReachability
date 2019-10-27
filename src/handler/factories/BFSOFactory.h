//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_BFSOFACTORY_H
#define ALLPAIRREACH_BFSOFACTORY_H


#include <vector>
#include "../../algorithm.reach.allPair/DynamicAPReachAlgorithm.h"
#include "PartitionedFactory.h"

class BFSOFactory : public PartitionedFactory{
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

public:
    void setAdvancedBFSO(bool b);

    void setSimpleBFSO(bool b);

    bool advancedBFSO = false;
    bool simpleBFSO = false;
};


#endif //ALLPAIRREACH_BFSOFACTORY_H
