//
// Created by Alwin Stockinger.
//

#include "StaticBFSAPReachAlgorithm.h"

Algora::DynamicSSReachAlgorithm *StaticBFSAPReachAlgorithm::createAlgorithm() {
    return new Algora::StaticBFSSSReachAlgorithm();
}

StaticBFSAPReachAlgorithm::StaticBFSAPReachAlgorithm() : SSBasedDAPReachAlgorithm() {}
