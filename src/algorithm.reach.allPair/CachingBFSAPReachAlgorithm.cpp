//
// Created by Alwin Stockinger.
//

#include "CachingBFSAPReachAlgorithm.h"

std::string CachingBFSAPReachAlgorithm::getName() const noexcept {
    return std::__cxx11::string("All Pair Reachability Algorithm based on Caching BFS Single-Source Reachability");
}

std::string CachingBFSAPReachAlgorithm::getShortName() const noexcept {
    return std::__cxx11::string("AP based on Caching-BFS-SSReach");
}

SSBasedDAPReachAlgorithm::SSRAlgo *CachingBFSAPReachAlgorithm::createAlgorithm() {
    return new Algora::CachingBFSSSReachAlgorithm;
}
