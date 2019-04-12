//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_STATICDFSAPREACHALGORITHM_H
#define ALLPAIRREACH_STATICDFSAPREACHALGORITHM_H


#include "SSBasedDAPReachAlgorithm.h"

#include <algorithm.reach/staticdfsssreachalgorithm.h>

class StaticDFSAPReachAlgorithm : SSBasedDAPReachAlgorithm{

public:
    explicit StaticDFSAPReachAlgorithm() : SSBasedDAPReachAlgorithm(){}

private:

    SSRAlgo *createAlgorithm() override;

    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;

};


#endif //ALLPAIRREACH_STATICDFSAPREACHALGORITHM_H
