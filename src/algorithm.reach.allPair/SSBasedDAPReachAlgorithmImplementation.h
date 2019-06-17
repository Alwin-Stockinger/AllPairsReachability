//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_SSBASEDDAPREACHALGORITHMIMPLEMENTATION_H
#define ALLPAIRREACH_SSBASEDDAPREACHALGORITHMIMPLEMENTATION_H


#include <algorithm.reach/dynamicssreachalgorithm.h>
#include "SSBasedDAPReachAlgorithm.h"

template <class T>
class SSBasedDAPReachAlgorithmImplementation : public SSBasedDAPReachAlgorithm {
    static_assert(std::is_base_of<Algora::DynamicSSReachAlgorithm,T>::value, "Template Parameter has to inherit from Algora::DynamicSSReachAlgorithm");

    Algora::DynamicSSReachAlgorithm *createSSAlgorithm() override {
        return new T();
    }
};


#endif //ALLPAIRREACH_SSBASEDDAPREACHALGORITHMIMPLEMENTATION_H
