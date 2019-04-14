//
// Created by Alwin Stockinger.
//

#include "SimpleIncAPReachAlgorithm.h"

std::string SimpleIncAPReachAlgorithm::getName() const noexcept {
    return std::__cxx11::string();
}

std::string SimpleIncAPReachAlgorithm::getShortName() const noexcept {
    return std::__cxx11::string("AP based on Simple-Inc-SSReach");
}

SSBasedDAPReachAlgorithm::SSRAlgo *SimpleIncAPReachAlgorithm::createAlgorithm() {
    auto *algorithm = new Algora::SimpleIncSSReachAlgorithm(reverse, searchForward, maxUS, radicalReset);

    if (maxUSLog) algorithm->setMaxUnknownStateLog();
    if (maxUSSqrt) algorithm->setMaxUnknownStateSqrt();
    algorithm->relateToReachableVertices(relateToReachable);

    return algorithm;
}

SimpleIncAPReachAlgorithm::SimpleIncAPReachAlgorithm(bool reverse, bool searchForward, double maxUS,
                                                     bool radicalReset)
        : SSBasedDAPReachAlgorithm(), reverse(reverse), searchForward(searchForward), maxUS(maxUS),
          radicalReset(radicalReset), relateToReachable(false), maxUSLog(false), maxUSSqrt(false) {}

void SimpleIncAPReachAlgorithm::relateToReachableVertices(bool rTR) {
    this->relateToReachable = rTR;
}

void SimpleIncAPReachAlgorithm::setMaxUnknownStateSqrt() {
    this->maxUSSqrt = true;
}

void SimpleIncAPReachAlgorithm::setMaxUnknownStateLog() {
    this->maxUSLog = true;
}

