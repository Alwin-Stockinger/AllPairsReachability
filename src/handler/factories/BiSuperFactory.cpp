//
// Created by Alwin Stockinger.
//

#include "BiSuperFactory.h"
#include "../../algorithm.reach.allPair/SSBasedDAPReachAlgorithmImplementation.h"
#include "../../algorithm.reach.allPair/BidirectionalSuperVertexAlgorithmImplementation.h"

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

std::vector<DynamicAPReachAlgorithm *> BiSuperFactory::getStaticBFSAlgorithms() {
    return createAlgorithms<Algora::StaticBFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> BiSuperFactory::getStaticDFSAlgorithms() {
    return createAlgorithms<Algora::StaticDFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> BiSuperFactory::getCachingBFSAlgorithms() {
    return createAlgorithms<Algora::CachingBFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> BiSuperFactory::getCachingDFSAlgorithms() {
    return createAlgorithms<Algora::CachingDFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> BiSuperFactory::getLazyBFSAlgorithms() {
    return createAlgorithms<Algora::LazyBFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> BiSuperFactory::getLazyDFSAlgorithms() {
    return createAlgorithms<Algora::LazyDFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> BiSuperFactory::getSimpleISSRAlgorithms() {
    return createAlgorithms<Algora::SimpleIncSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> BiSuperFactory::getSimpleESTAlgorithms() {
    return createAlgorithms<Algora::SimpleESTree>();
}

std::vector<DynamicAPReachAlgorithm *> BiSuperFactory::getMLESTAlgorithms() {
    return createAlgorithms<Algora::ESTreeML>();
}

std::vector<DynamicAPReachAlgorithm *> BiSuperFactory::getESTAlgorithms() {
    return createAlgorithms<Algora::ESTreeQ>();
}

std::vector<DynamicAPReachAlgorithm *> BiSuperFactory::getOldESTAlgorithms() {
    return createAlgorithms<Algora::OldESTree>();
}

template<typename SubAlgorithm>
std::vector<DynamicAPReachAlgorithm *> BiSuperFactory::createAlgorithms() {
    std::vector<DynamicAPReachAlgorithm*> algorithms;

    for(unsigned long long k = kMin; k <= kMax ;  [this, &k]() {
        exponentialK ? (k *= 2ULL) : (k++);
    }()) {
        for(unsigned long long depth = depthMin; depth <= depthMax; depth++){

            for(unsigned long long stepSize = minSteps; stepSize <= maxSteps; stepSize++){
                auto* algorithm = new BidirectionalSuperVertexAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<SubAlgorithm, true>>(depth);

                configureAlgorithm(algorithm);
                algorithm->setK(k);
                algorithm->setStepSize(stepSize);
                algorithms.push_back(algorithm);
            }
        }
    }
    return algorithms;
}