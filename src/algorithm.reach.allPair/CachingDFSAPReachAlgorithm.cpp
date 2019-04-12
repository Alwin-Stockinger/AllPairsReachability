//
// Created by Alwin Stockinger.
//

#include "CachingDFSAPReachAlgorithm.h"

std::string CachingDFSAPReachAlgorithm::getName() const noexcept {
    return std::__cxx11::string("All Pair Reachability Algorithm based on Caching DFS Single-Source Reachability");
}

std::string CachingDFSAPReachAlgorithm::getShortName() const noexcept {
    return std::__cxx11::string("AP based on Caching-BFS-SSReach");
}

SSBasedDAPReachAlgorithm::SSRAlgo *CachingDFSAPReachAlgorithm::createAlgorithm() {
    return new Algora::CachingDFSSSReachAlgorithm;
}
