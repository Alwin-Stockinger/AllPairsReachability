//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_REVERSEBFSFACTORY_H
#define ALLPAIRREACH_REVERSEBFSFACTORY_H


#include "AlgorithmFactory.h"
#include "PartitionedFactory.h"

class ReverseBFSFactory : public PartitionedFactory{
public:
    std::vector<DynamicAPReachAlgorithm *> getAlgorithms() override;


    void setAdvancedBFSO(bool b) {
        this->advancedBFSO = b;
    }

    void setSimpleBFSO(bool b) {
        this->simpleBFSO = b;
    }


    void setMinSteps(unsigned long long newMinSteps){
        minSteps = newMinSteps;
    }

    void setMaxSteps(unsigned long long newMaxSteps){
        maxSteps = newMaxSteps;
    }

private:

    unsigned long long minSteps = 2ULL;
    unsigned long long maxSteps = 2ULL;

    bool advancedBFSO = false;
    bool simpleBFSO = false;

private:

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

};


#endif //ALLPAIRREACH_REVERSEBFSFACTORY_H
