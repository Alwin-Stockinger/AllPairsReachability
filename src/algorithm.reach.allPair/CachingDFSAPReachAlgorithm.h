//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_CACHINGDFSAPREACHALGORITHM_H
#define ALLPAIRREACH_CACHINGDFSAPREACHALGORITHM_H


#include "SSBasedDAPReachAlgorithm.h"

class CachingDFSAPReachAlgorithm : SSBasedDAPReachAlgorithm{
    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;

    SSRAlgo *createAlgorithm() override;
};


#endif //ALLPAIRREACH_CACHINGDFSAPREACHALGORITHM_H
