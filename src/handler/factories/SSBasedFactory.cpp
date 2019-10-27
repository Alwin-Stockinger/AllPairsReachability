//
// Created by Alwin Stockinger.
//

#include <algorithm.reach/cachingbfsssreachalgorithm.h>
#include <algorithm.reach/cachingdfsssreachalgorithm.h>
#include <algorithm.reach/lazybfsssreachalgorithm.h>
#include <algorithm.reach/lazydfsssreachalgorithm.h>
#include <algorithm.reach/simpleincssreachalgorithm.h>
#include <algorithm.reach.es/simpleestree.h>
#include <algorithm.reach.es/estree-ml.h>
#include <datastructure/bucketqueue.h>
#include <algorithm.reach.es/estree-queue.h>
#include <algorithm.reach.es/estree-bqueue.h>
#include "SSBasedFactory.h"
#include "../../algorithm.reach.allPair/SSBasedDAPReachAlgorithmImplementation.h"

std::vector<DynamicAPReachAlgorithm *> SSBasedFactory::getStaticBFSAlgorithms() {
    return {new SSBasedDAPReachAlgorithmImplementation<Algora::StaticBFSSSReachAlgorithm>};
}

std::vector<DynamicAPReachAlgorithm *> SSBasedFactory::getStaticDFSAlgorithms() {
    return {new SSBasedDAPReachAlgorithmImplementation<Algora::StaticDFSSSReachAlgorithm>};
}

std::vector<DynamicAPReachAlgorithm *> SSBasedFactory::getCachingBFSAlgorithms() {
    return {new SSBasedDAPReachAlgorithmImplementation<Algora::CachingBFSSSReachAlgorithm, true>};
}

std::vector<DynamicAPReachAlgorithm *> SSBasedFactory::getCachingDFSAlgorithms() {
    return {new SSBasedDAPReachAlgorithmImplementation<Algora::CachingDFSSSReachAlgorithm, true>};
}

std::vector<DynamicAPReachAlgorithm *> SSBasedFactory::getLazyBFSAlgorithms() {
    return {new SSBasedDAPReachAlgorithmImplementation<Algora::LazyBFSSSReachAlgorithm, true>};
}

std::vector<DynamicAPReachAlgorithm *> SSBasedFactory::getLazyDFSAlgorithms() {
    return {new SSBasedDAPReachAlgorithmImplementation<Algora::LazyDFSSSReachAlgorithm, true>};
}

std::vector<DynamicAPReachAlgorithm *> SSBasedFactory::getSimpleISSRAlgorithms() {
    return {new SSBasedDAPReachAlgorithmImplementation<Algora::SimpleIncSSReachAlgorithm, true>};
}

std::vector<DynamicAPReachAlgorithm *> SSBasedFactory::getSimpleESTAlgorithms() {
    return {new SSBasedDAPReachAlgorithmImplementation<Algora::SimpleESTree, true>};
}

std::vector<DynamicAPReachAlgorithm *> SSBasedFactory::getMLESTAlgorithms() {
    return {new SSBasedDAPReachAlgorithmImplementation<Algora::ESTreeML, true>};
}

std::vector<DynamicAPReachAlgorithm *> SSBasedFactory::getESTAlgorithms() {
    return {new SSBasedDAPReachAlgorithmImplementation<Algora::ESTreeQ, true>};
}

std::vector<DynamicAPReachAlgorithm *> SSBasedFactory::getOldESTAlgorithms() {
    return {new SSBasedDAPReachAlgorithmImplementation<Algora::OldESTree, true>};
}
