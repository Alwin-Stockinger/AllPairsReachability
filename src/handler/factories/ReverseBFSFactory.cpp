//
// Created by Alwin Stockinger.
//

#include "ReverseBFSFactory.h"
#include "../../algorithm.reach.allPair/ReverseBFSPartitionedAPReachAlgorithm.h"


std::vector<DynamicAPReachAlgorithm *> ReverseBFSFactory::getAlgorithms() {
    std::vector<DynamicAPReachAlgorithm*> algorithms;
    for(unsigned long long k = kMin; k <= kMax ;  [this, &k]() {
        exponentialK ? (k *= 2ULL) : (k++);
    }()){
        if (fullyReverse) for(unsigned long long steps = minSteps; steps <= maxSteps; steps++){
            if (advancedBFSO) {
                auto *algorithm = new ReverseBFSPartitionedAPReachAlgorithm;

                configureAlgorithm(algorithm);
                algorithm->setK(k);
                algorithm->setSetRemovals(true);
                algorithm->setStepSize(steps);

                algorithms.push_back(algorithm);
            }
            if (simpleBFSO) {
                auto *algorithm = new ReverseBFSPartitionedAPReachAlgorithm;

                configureAlgorithm(algorithm);
                algorithm->setK(k);
                algorithm->setSetRemovals(false);
                algorithm->setStepSize(steps);

                algorithms.push_back(algorithm);
            }
        }
        else{
            if (advancedBFSO) {
                auto *algorithm = new ReverseBFSPartitionedAPReachAlgorithm;

                configureAlgorithm(algorithm);
                algorithm->setK(k);
                algorithm->setSetRemovals(true);

                algorithms.push_back(algorithm);
            }
            if (simpleBFSO) {
                auto *algorithm = new ReverseBFSPartitionedAPReachAlgorithm;

                configureAlgorithm(algorithm);
                algorithm->setK(k);
                algorithm->setSetRemovals(false);

                algorithms.push_back(algorithm);
            }
        }
    }
    return algorithms;
}

void ReverseBFSFactory::setFullyReverseBFS(bool b) {
    fullyReverse = b;
}

std::vector<DynamicAPReachAlgorithm *> ReverseBFSFactory::getStaticBFSAlgorithms() {
    throw std::logic_error("Can't make SS-algorithm for Reverse BFS");
}

std::vector<DynamicAPReachAlgorithm *> ReverseBFSFactory::getStaticDFSAlgorithms() {
    throw std::logic_error("Can't make SS-algorithm for Reverse BFS");
}

std::vector<DynamicAPReachAlgorithm *> ReverseBFSFactory::getCachingBFSAlgorithms() {
    throw std::logic_error("Can't make SS-algorithm for Reverse BFS");
}

std::vector<DynamicAPReachAlgorithm *> ReverseBFSFactory::getCachingDFSAlgorithms() {
    throw std::logic_error("Can't make SS-algorithm for Reverse BFS");
}

std::vector<DynamicAPReachAlgorithm *> ReverseBFSFactory::getLazyBFSAlgorithms() {
    throw std::logic_error("Can't make SS-algorithm for Reverse BFS");
}

std::vector<DynamicAPReachAlgorithm *> ReverseBFSFactory::getLazyDFSAlgorithms() {
    throw std::logic_error("Can't make SS-algorithm for Reverse BFS");
}

std::vector<DynamicAPReachAlgorithm *> ReverseBFSFactory::getSimpleISSRAlgorithms() {
    throw std::logic_error("Can't make SS-algorithm for Reverse BFS");
}

std::vector<DynamicAPReachAlgorithm *> ReverseBFSFactory::getSimpleESTAlgorithms() {
    throw std::logic_error("Can't make SS-algorithm for Reverse BFS");
}

std::vector<DynamicAPReachAlgorithm *> ReverseBFSFactory::getMLESTAlgorithms() {
    throw std::logic_error("Can't make SS-algorithm for Reverse BFS");}

std::vector<DynamicAPReachAlgorithm *> ReverseBFSFactory::getESTAlgorithms() {
    throw std::logic_error("Can't make SS-algorithm for Reverse BFS");}

std::vector<DynamicAPReachAlgorithm *> ReverseBFSFactory::getOldESTAlgorithms() {
    throw std::logic_error("Can't make SS-algorithm for Reverse BFS");}
