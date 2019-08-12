//
// Created by Alwin Stockinger.
//

#include <algorithm.reach/lazydfsssreachalgorithm.h>
#include <algorithm.reach.es/simpleestree.h>
#include <algorithm.reach.es/estree-queue.h>
#include <algorithm.reach.es/estree-ml.h>
#include <algorithm.reach.es/estree-bqueue.h>
#include <algorithm.reach/lazybfsssreachalgorithm.h>
#include <algorithm.reach/cachingdfsssreachalgorithm.h>
#include <algorithm.reach/cachingbfsssreachalgorithm.h>
#include <algorithm.reach/simpleincssreachalgorithm.h>
#include "SimplePartitionFactory.h"
#include "../../algorithm.reach.allPair/SSBasedDAPReachAlgorithm.h"
#include "../../algorithm.reach.allPair/SSBasedDAPReachAlgorithmImplementation.h"
#include "../../algorithm.reach.allPair/SimplePartitionedDAPReachAlgorithmImplementation.h"

std::vector<DynamicAPReachAlgorithm *> SimplePartitionFactory::getStaticBFSAlgorithms() {
    return createAlgorithms<Algora::StaticBFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> SimplePartitionFactory::getStaticDFSAlgorithms() {
    return createAlgorithms<Algora::StaticDFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> SimplePartitionFactory::getCachingBFSAlgorithms() {
    return createAlgorithms<Algora::CachingBFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> SimplePartitionFactory::getCachingDFSAlgorithms() {
    return createAlgorithms<Algora::CachingDFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> SimplePartitionFactory::getLazyBFSAlgorithms() {
    return createAlgorithms<Algora::LazyBFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> SimplePartitionFactory::getLazyDFSAlgorithms() {
    return createAlgorithms<Algora::LazyDFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> SimplePartitionFactory::getSimpleISSRAlgorithms() {
    return createAlgorithms<Algora::SimpleIncSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> SimplePartitionFactory::getSimpleESTAlgorithms() {
    return createAlgorithms<Algora::SimpleESTree>();
}

std::vector<DynamicAPReachAlgorithm *> SimplePartitionFactory::getMLESTAlgorithms() {
    return createAlgorithms<Algora::ESTreeML>();
}

std::vector<DynamicAPReachAlgorithm *> SimplePartitionFactory::getESTAlgorithms() {
    return createAlgorithms<Algora::ESTreeQ>();
}

std::vector<DynamicAPReachAlgorithm *> SimplePartitionFactory::getOldESTAlgorithms() {
    return createAlgorithms<Algora::OldESTree>();
}

template<typename SubAlgorithm>
std::vector<DynamicAPReachAlgorithm *> SimplePartitionFactory::createAlgorithms() {

    std::vector<DynamicAPReachAlgorithm*> algorithms;

    for(const std::string& overlayName : overlayNames) {

        std::vector<DynamicAPReachAlgorithm*> specificAlgorithms;

        if (overlayName == "StaticBFS") {
            specificAlgorithms =
                    createSpecificAlgorithms<SubAlgorithm, Algora::StaticBFSSSReachAlgorithm>();
        } else if (overlayName == "StaticDFS") {
            specificAlgorithms =
                    createSpecificAlgorithms <SubAlgorithm, Algora::StaticDFSSSReachAlgorithm >();
        } else if (overlayName == "LazyDFS") {
            specificAlgorithms =
                    createSpecificAlgorithms <SubAlgorithm, Algora::LazyDFSSSReachAlgorithm> ();
        } else if (overlayName == "LazyBFS") {
            specificAlgorithms =
                    createSpecificAlgorithms <SubAlgorithm, Algora::LazyBFSSSReachAlgorithm> ();
        } else if (overlayName == "CachingDFS") {
            specificAlgorithms =
                    createSpecificAlgorithms <SubAlgorithm,Algora::CachingDFSSSReachAlgorithm> ();
        } else if (overlayName == "CachingBFS") {
            specificAlgorithms =
                    createSpecificAlgorithms <SubAlgorithm, Algora::CachingBFSSSReachAlgorithm> ();
        } else if (overlayName == "SimpleInc") {
            specificAlgorithms =
                    createSpecificAlgorithms <SubAlgorithm, Algora::SimpleIncSSReachAlgorithm> ();
        } else if (overlayName == "ESTreeML") {
            specificAlgorithms =
                    createSpecificAlgorithms <SubAlgorithm, Algora::ESTreeML> ();
        } else if (overlayName == "OldESTree") {
            specificAlgorithms =
                    createSpecificAlgorithms <SubAlgorithm, Algora::OldESTree> ();
        } else if (overlayName == "ESTreeQ") {
            specificAlgorithms =
                    createSpecificAlgorithms <SubAlgorithm, Algora::ESTreeQ> ();
        } else if (overlayName == "SimpleESTree") {
            specificAlgorithms =
                    createSpecificAlgorithms <SubAlgorithm, Algora::SimpleESTree> ();
        } else {
            throw std::invalid_argument(overlayName + " is not a viable algorithm");
        }

        algorithms.insert(algorithms.end(), specificAlgorithms.begin(), specificAlgorithms.end());
    }
    return algorithms;
}

template<typename SubAlgorithm, typename OverlayAlgorithm>
std::vector<DynamicAPReachAlgorithm *> SimplePartitionFactory::createSpecificAlgorithms() {

    std::vector<DynamicAPReachAlgorithm*> algorithms;

    for(unsigned long long k = kMin; k <= kMax ;  [this, &k]() {
        exponentialK ? (k *= 2ULL) : (k++);
    }()) {
        for(unsigned long long depth = depthMin; depth <= depthMax; depth++){
            auto* algorithm = new SimplePartitionedDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<SubAlgorithm, false>, SSBasedDAPReachAlgorithmImplementation<OverlayAlgorithm, false>>(depth);

            configureAlgorithm(algorithm);
            algorithm->setK(k);

            algorithms.push_back(algorithm);
        }
    }
    return algorithms;
}