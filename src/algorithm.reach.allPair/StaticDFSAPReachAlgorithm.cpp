//
// Created by Alwin Stockinger.
//

#include "StaticDFSAPReachAlgorithm.h"

SSBasedDAPReachAlgorithm::SSRAlgo *StaticDFSAPReachAlgorithm::createAlgorithm() {
    return new Algora::StaticDFSSSReachAlgorithm;
}

std::string StaticDFSAPReachAlgorithm::getName() const noexcept {
    return std::__cxx11::string("All Pair Reachability Algorithm based on Static DFS Single-Source Reachability");
}

std::string StaticDFSAPReachAlgorithm::getShortName() const noexcept {
    return std::__cxx11::string("AP based on Static-DFS-SSReach");
}
