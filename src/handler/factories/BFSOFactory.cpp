//
// Created by Alwin Stockinger.
//

#include <algorithm.reach/lazydfsssreachalgorithm.h>
#include <algorithm.reach/lazybfsssreachalgorithm.h>
#include <algorithm.reach.es/simpleestree.h>
#include <algorithm.reach.es/estree-queue.h>
#include <algorithm.reach.es/estree-bqueue.h>
#include <algorithm.reach.es/estree-ml.h>
#include <algorithm.reach/simpleincssreachalgorithm.h>
#include <algorithm.reach/cachingbfsssreachalgorithm.h>
#include <algorithm.reach/cachingdfsssreachalgorithm.h>
#include "BFSOFactory.h"
#include "../../algorithm.reach.allPair/SSBasedDAPReachAlgorithmImplementation.h"
#include "../../algorithm.reach.allPair/BFSOPDAPReachAlgorithmImplementation.h"

std::vector<DynamicAPReachAlgorithm *> BFSOFactory::getStaticBFSAlgorithms() {
    return createAlgorithms<Algora::StaticBFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> BFSOFactory::getStaticDFSAlgorithms() {
    return createAlgorithms<Algora::StaticDFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> BFSOFactory::getCachingBFSAlgorithms() {
    return createAlgorithms<Algora::CachingBFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> BFSOFactory::getCachingDFSAlgorithms() {
    return createAlgorithms<Algora::CachingDFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> BFSOFactory::getLazyBFSAlgorithms() {
    return createAlgorithms<Algora::LazyBFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> BFSOFactory::getLazyDFSAlgorithms() {
    return createAlgorithms<Algora::LazyDFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> BFSOFactory::getSimpleISSRAlgorithms() {
    return createAlgorithms<Algora::SimpleIncSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> BFSOFactory::getSimpleESTAlgorithms() {
    return createAlgorithms<Algora::SimpleESTree>();
}

std::vector<DynamicAPReachAlgorithm *> BFSOFactory::getMLESTAlgorithms() {
    return createAlgorithms<Algora::ESTreeML>();
}

std::vector<DynamicAPReachAlgorithm *> BFSOFactory::getESTAlgorithms() {
    return createAlgorithms<Algora::ESTreeQ>();
}

std::vector<DynamicAPReachAlgorithm *> BFSOFactory::getOldESTAlgorithms() {
    return createAlgorithms<Algora::OldESTree>();
}

template<typename SubAlgorithm>
std::vector<DynamicAPReachAlgorithm *> BFSOFactory::createAlgorithms() {
    std::vector<DynamicAPReachAlgorithm*> algorithms;

    for(unsigned long long k = kMin; k <= kMax ;  [this, &k]() {
        exponentialK ? (k *= 2ULL) : (k++);
    }()) {
        for(unsigned long long depth = depthMin; depth <= depthMax; depth++){
            if(advancedBFSO){
                auto* algorithm = new BFSOPDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<SubAlgorithm, true>>(depth);

                configureAlgorithm(algorithm);
                algorithm->setK(k);
                algorithm->setSetRemovals(true);

                algorithms.push_back(algorithm);
            }
            if(simpleBFSO){
                auto* algorithm = new BFSOPDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<SubAlgorithm, true>>(depth);

                configureAlgorithm(algorithm);
                algorithm->setK(k);
                algorithm->setSetRemovals(false);

                algorithms.push_back(algorithm);
            }

        }
    }
    return algorithms;
}

void BFSOFactory::setAdvancedBFSO(bool b) {
    advancedBFSO = b;
}

void BFSOFactory::setSimpleBFSO(bool b){
    simpleBFSO = b;
}
