//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_BISUPERFACTORY_H
#define ALLPAIRREACH_BISUPERFACTORY_H


#include "PartitionedFactory.h"

class BiSuperFactory : public PartitionedFactory{
protected:
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

    unsigned long long maxSteps = 0ULL;
    unsigned long long minSteps = 0ULL;

public:

    void setMaxSteps(unsigned long long steps){
        maxSteps = steps;
    }

    void setMinSteps(unsigned long long steps){
        minSteps = steps;
    }

};


#endif //ALLPAIRREACH_BISUPERFACTORY_H
