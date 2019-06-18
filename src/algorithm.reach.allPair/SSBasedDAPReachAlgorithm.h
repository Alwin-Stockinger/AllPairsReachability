//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_SSBASEDDAPREACHALGORITHM_H
#define ALLPAIRREACH_SSBASEDDAPREACHALGORITHM_H


#include "DynamicAPReachAlgorithm.h"

class SSBasedDAPReachAlgorithm : public DynamicAPReachAlgorithm{

protected:
    virtual Algora::DynamicSSReachAlgorithm* createSSAlgorithm() = 0;

};


#endif //ALLPAIRREACH_SSBASEDDAPREACHALGORITHM_H
