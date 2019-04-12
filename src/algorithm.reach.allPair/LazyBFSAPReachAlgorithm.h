//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_LAZYBFSAPREACHALGORITHM_H
#define ALLPAIRREACH_LAZYBFSAPREACHALGORITHM_H


#include "SSBasedDAPReachAlgorithm.h"

#include <algorithm.reach/lazybfsssreachalgorithm.h>

class LazyBFSAPReachAlgorithm : SSBasedDAPReachAlgorithm{
    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;

    SSRAlgo *createAlgorithm() override;

};


#endif //ALLPAIRREACH_LAZYBFSAPREACHALGORITHM_H
