//
// Created by Alwin Stockinger.
//

#include "DFSOFactory.h"

#include <algorithm.reach/lazydfsssreachalgorithm.h>
#include <algorithm.reach/lazybfsssreachalgorithm.h>
#include <algorithm.reach.es/simpleestree.h>
#include <algorithm.reach.es/estree-queue.h>
#include <algorithm.reach.es/estree-bqueue.h>
#include <algorithm.reach.es/estree-ml.h>
#include <algorithm.reach/simpleincssreachalgorithm.h>
#include <algorithm.reach/cachingbfsssreachalgorithm.h>
#include <algorithm.reach/cachingdfsssreachalgorithm.h>
#include <algorithm.reach/staticbfsssreachalgorithm.h>
#include <algorithm.reach/staticdfsssreachalgorithm.h>

#include "../../algorithm.reach.allPair/DSOPDAPReachAlgorithmImplementation.h"
#include "../../algorithm.reach.allPair/SSBasedDAPReachAlgorithmImplementation.h"

std::vector<DynamicAPReachAlgorithm *> DFSOFactory::getStaticBFSAlgorithms() {
    return createAlgorithms<Algora::StaticBFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> DFSOFactory::getStaticDFSAlgorithms() {
    return createAlgorithms<Algora::StaticDFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> DFSOFactory::getCachingBFSAlgorithms() {
    return createAlgorithms<Algora::CachingBFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> DFSOFactory::getCachingDFSAlgorithms() {
    return createAlgorithms<Algora::CachingDFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> DFSOFactory::getLazyBFSAlgorithms() {
    return createAlgorithms<Algora::LazyBFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> DFSOFactory::getLazyDFSAlgorithms() {
    return createAlgorithms<Algora::LazyDFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> DFSOFactory::getSimpleISSRAlgorithms() {
    return createAlgorithms<Algora::SimpleIncSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> DFSOFactory::getSimpleESTAlgorithms() {
    return createAlgorithms<Algora::SimpleESTree>();
}

std::vector<DynamicAPReachAlgorithm *> DFSOFactory::getMLESTAlgorithms() {
    return createAlgorithms<Algora::ESTreeML>();
}

std::vector<DynamicAPReachAlgorithm *> DFSOFactory::getESTAlgorithms() {
    return createAlgorithms<Algora::ESTreeQ>();
}

std::vector<DynamicAPReachAlgorithm *> DFSOFactory::getOldESTAlgorithms() {
    return createAlgorithms<Algora::OldESTree>();
}

template<typename SubAlgorithm>
std::vector<DynamicAPReachAlgorithm *> DFSOFactory::createAlgorithms() {
    std::vector<DynamicAPReachAlgorithm*> algorithms;

    for(unsigned long long k = kMin; k <= kMax ;  [this, &k]() {
        exponentialK ? (k *= 2ULL) : (k++);
    }()) {
        for(unsigned long long depth = depthMin; depth <= depthMax; depth++){
            if(advancedOverlaySearch){
                auto* algorithm = new DFSOPDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<SubAlgorithm, true>>(depth);

                configureAlgorithm(algorithm);
                algorithm->setK(k);
                algorithm->setSetRemovals(true);

                algorithms.push_back(algorithm);
            }
            if(simpleOverlaySearch){
                auto* algorithm = new DFSOPDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<SubAlgorithm, true>>(depth);

                configureAlgorithm(algorithm);
                algorithm->setK(k);
                algorithm->setSetRemovals(false);

                algorithms.push_back(algorithm);
            }

        }
    }
    return algorithms;
}

void DFSOFactory::setAdvancedDFSO(bool b) {
    advancedOverlaySearch = b;
}

void DFSOFactory::setSimpleDFSO(bool b){
    simpleOverlaySearch = b;
}
