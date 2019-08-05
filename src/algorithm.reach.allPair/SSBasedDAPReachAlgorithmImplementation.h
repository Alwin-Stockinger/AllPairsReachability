//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_SSBASEDDAPREACHALGORITHMIMPLEMENTATION_H
#define ALLPAIRREACH_SSBASEDDAPREACHALGORITHMIMPLEMENTATION_H


#include <algorithm.reach/dynamicssreachalgorithm.h>
#include <algorithm.reach/staticdfsssreachalgorithm.h>
#include <algorithm.reach/staticbfsssreachalgorithm.h>
#include "DynamicSSBasedDAPReachAlgorithm.h"
#include "StaticSSBasedDAPReachAlgorithm.h"


template <class T, bool immediateInit = true>
class SSBasedDAPReachAlgorithmImplementation : public DynamicSSBasedDAPReachAlgorithm<immediateInit> {
    static_assert(std::is_base_of<Algora::DynamicSSReachAlgorithm,T>::value, "Template Parameter has to inherit from Algora::DynamicSSReachAlgorithm");

    Algora::DynamicSSReachAlgorithm *createSSAlgorithm() override {
        return new T();
    }
};

template <>
class SSBasedDAPReachAlgorithmImplementation <Algora::StaticDFSSSReachAlgorithm> : public StaticSSBasedDAPReachAlgorithm{
    Algora::DynamicSSReachAlgorithm *createSSAlgorithm() override {
        return new Algora::StaticDFSSSReachAlgorithm;
    }
};

template <>
class SSBasedDAPReachAlgorithmImplementation <Algora::StaticBFSSSReachAlgorithm> : public StaticSSBasedDAPReachAlgorithm{
    Algora::DynamicSSReachAlgorithm *createSSAlgorithm() override {
        return new Algora::StaticBFSSSReachAlgorithm;
    }
};


#endif //ALLPAIRREACH_SSBASEDDAPREACHALGORITHMIMPLEMENTATION_H
