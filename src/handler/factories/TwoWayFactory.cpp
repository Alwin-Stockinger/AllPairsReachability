//
// Created by Alwin Stockinger.
//

#include "TwoWayFactory.h"
#include "../../algorithm.reach.allPair/TwoWayBFSAPReachAlgorithm.h"

std::vector<DynamicAPReachAlgorithm *> TwoWayFactory::getStaticBFSAlgorithms() {
    return std::vector<DynamicAPReachAlgorithm *>();
}

std::vector<DynamicAPReachAlgorithm *> TwoWayFactory::getStaticDFSAlgorithms() {
    return std::vector<DynamicAPReachAlgorithm *>();
}

std::vector<DynamicAPReachAlgorithm *> TwoWayFactory::getCachingBFSAlgorithms() {
    return std::vector<DynamicAPReachAlgorithm *>();
}

std::vector<DynamicAPReachAlgorithm *> TwoWayFactory::getCachingDFSAlgorithms() {
    return std::vector<DynamicAPReachAlgorithm *>();
}

std::vector<DynamicAPReachAlgorithm *> TwoWayFactory::getLazyBFSAlgorithms() {
    return std::vector<DynamicAPReachAlgorithm *>();
}

std::vector<DynamicAPReachAlgorithm *> TwoWayFactory::getLazyDFSAlgorithms() {
    return std::vector<DynamicAPReachAlgorithm *>();
}

std::vector<DynamicAPReachAlgorithm *> TwoWayFactory::getSimpleISSRAlgorithms() {
    return std::vector<DynamicAPReachAlgorithm *>();
}

std::vector<DynamicAPReachAlgorithm *> TwoWayFactory::getSimpleESTAlgorithms() {
    return std::vector<DynamicAPReachAlgorithm *>();
}

std::vector<DynamicAPReachAlgorithm *> TwoWayFactory::getMLESTAlgorithms() {
    return std::vector<DynamicAPReachAlgorithm *>();
}

std::vector<DynamicAPReachAlgorithm *> TwoWayFactory::getESTAlgorithms() {
    return std::vector<DynamicAPReachAlgorithm *>();
}

std::vector<DynamicAPReachAlgorithm *> TwoWayFactory::getOldESTAlgorithms() {
    return std::vector<DynamicAPReachAlgorithm *>();
}

std::vector<DynamicAPReachAlgorithm *> TwoWayFactory::getAlgorithms() {

    std::vector<DynamicAPReachAlgorithm*> algorithmVector;
    for(unsigned long long steps = minSteps; steps <= maxSteps ; steps++){
        auto* algorithm = new TwoWayBFSAPReachAlgorithm();
        algorithm->setStepSize(steps);
        algorithmVector.push_back(algorithm);
    }
    return algorithmVector;
}
