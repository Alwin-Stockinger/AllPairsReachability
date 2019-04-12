//
// Created by Alwin Stockinger.
//

#include "LazyBFSAPReachAlgorithm.h"

std::string LazyBFSAPReachAlgorithm::getName() const noexcept {
    return std::__cxx11::string("All Pair Reachability Algorithm based on Lazy BFS Single-Source Reachability");
}

std::string LazyBFSAPReachAlgorithm::getShortName() const noexcept {
    return std::__cxx11::string("AP based on Lazy-BFS-SSReach");
}

SSBasedDAPReachAlgorithm::SSRAlgo *LazyBFSAPReachAlgorithm::createAlgorithm() {
    return new Algora::LazyBFSSSReachAlgorithm;
}
