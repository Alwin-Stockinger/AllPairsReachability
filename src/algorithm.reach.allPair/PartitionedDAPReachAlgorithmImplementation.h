//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHMIMPLEMENTATION_H
#define ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHMIMPLEMENTATION_H

#include <map>
#include <set>
#include <property/propertymap.h>

#include "DynamicAPReachAlgorithm.h"
#include "DynamicSSBasedDAPReachAlgorithm.h"
#include "PartitionedDAPReachAlgorithm.h"

template <typename T, bool propagatePartitionFunction = true>
class PartitionedDAPReachAlgorithmImplementation : public PartitionedDAPReachAlgorithm{

public:
    static_assert(std::is_base_of<DynamicAPReachAlgorithm,T>::value, "Template Parameter has to inherit from DynamicApAlgorithm");

    explicit PartitionedDAPReachAlgorithmImplementation(const unsigned depth = 0U) : PartitionedDAPReachAlgorithm(), depth(depth){};


    ~PartitionedDAPReachAlgorithmImplementation() override = default;

private:

    const unsigned depth = 0U;

    DynamicAPReachAlgorithm *createSubAlgorithm() override {
        if( depth > 0U){
            auto* newAlgo = new PartitionedDAPReachAlgorithmImplementation<T>(depth - 1);
            if(propagatePartitionFunction){
                newAlgo->setPartitionFunction(partitionFunction, k);
            }
            return newAlgo;
        } else{
            return new T;
        }
    }
};
#endif //ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H
