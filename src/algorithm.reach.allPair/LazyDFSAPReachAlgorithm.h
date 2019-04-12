//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_LAZYDFSAPREACHALGORITHM_H
#define ALLPAIRREACH_LAZYDFSAPREACHALGORITHM_H


#include "SSBasedDAPReachAlgorithm.h"

#include <algorithm.reach/lazydfsssreachalgorithm.cpp>

class LazyDFSAPReachAlgorithm : SSBasedDAPReachAlgorithm{
    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;

    SSRAlgo *createAlgorithm() override;
};


#endif //ALLPAIRREACH_LAZYDFSAPREACHALGORITHM_H
