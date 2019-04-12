//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_SIMPLEINCAPREACHALGORITHM_H
#define ALLPAIRREACH_SIMPLEINCAPREACHALGORITHM_H

#include <algorithm.reach/simpleincssreachalgorithm.h>
#include "SSBasedDAPReachAlgorithm.h"


class SimpleIncAPReachAlgorithm : SSBasedDAPReachAlgorithm{
    std::string getName() const override;

    std::string getShortName() const override;

    SSRAlgo *createAlgorithm() override;

};


#endif //ALLPAIRREACH_SIMPLEINCAPREACHALGORITHM_H
