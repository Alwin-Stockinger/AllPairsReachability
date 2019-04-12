//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_CACHINGBFSAPREACHALGORITHM_H
#define ALLPAIRREACH_CACHINGBFSAPREACHALGORITHM_H


#include "SSBasedDAPReachAlgorithm.h"

#include <algorithm.reach/cachingbfsssreachalgorithm.h>

class CachingBFSAPReachAlgorithm : SSBasedDAPReachAlgorithm{
    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;

    SSRAlgo *createAlgorithm() override;
};


#endif //ALLPAIRREACH_CACHINGBFSAPREACHALGORITHM_H
