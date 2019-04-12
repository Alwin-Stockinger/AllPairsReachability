//
// Created by Alwin Stockinger.
//

#include "LazyDFSAPReachAlgorithm.h"

std::string LazyDFSAPReachAlgorithm::getName() const noexcept {
    return std::__cxx11::string("All Pair Reachability Algorithm based on Lazy DFS Single-Source Reachability");
}

std::string LazyDFSAPReachAlgorithm::getShortName() const noexcept {
    return std::__cxx11::string("AP based on Lazy-DFS-SSReach");
}

SSBasedDAPReachAlgorithm::SSRAlgo *LazyDFSAPReachAlgorithm::createAlgorithm() {
    return new Algora::LazyDFSSSReachAlgorithm;
}
