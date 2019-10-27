//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_TWOWAYFACTORY_H
#define ALLPAIRREACH_TWOWAYFACTORY_H


#include "AlgorithmFactory.h"

class TwoWayFactory : public AlgorithmFactory{

public:
    std::vector<DynamicAPReachAlgorithm *> getAlgorithms() override;


    void setMinSteps(unsigned long long newMinSteps){
        minSteps = newMinSteps;
    }

    void setMaxSteps(unsigned long long newMaxSteps){
        maxSteps = newMaxSteps;
    }

private:

    unsigned long long minSteps = 2ULL;
    unsigned long long maxSteps = 2ULL;

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


#endif //ALLPAIRREACH_TWOWAYFACTORY_H
