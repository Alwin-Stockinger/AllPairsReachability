//
// Created by Alwin Stockinger.
//

#include "SimpleIncAPReachAlgorithm.h"

std::string SimpleIncAPReachAlgorithm::getName() const {
    return std::__cxx11::string();
}

std::string SimpleIncAPReachAlgorithm::getShortName() const {
    return std::__cxx11::string("AP based on Simple-Inc-SSReach");
}

SSBasedDAPReachAlgorithm::SSRAlgo *SimpleIncAPReachAlgorithm::createAlgorithm() {
    return nullptr;
}
