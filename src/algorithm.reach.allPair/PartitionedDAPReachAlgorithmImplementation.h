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

template <typename SubAlgorithm, typename OverlayAlgorithm = SubAlgorithm, bool propagatePartitionFunction = true>
class PartitionedDAPReachAlgorithmImplementation : public PartitionedDAPReachAlgorithm{

public:
    static_assert(std::is_base_of<DynamicAPReachAlgorithm,SubAlgorithm>::value, "Template Parameter has to inherit from DynamicApAlgorithm");
    static_assert(std::is_base_of<DynamicAPReachAlgorithm,OverlayAlgorithm>::value, "Template Parameter has to inherit from DynamicApAlgorithm");

    explicit PartitionedDAPReachAlgorithmImplementation(const unsigned depth = 0U) : PartitionedDAPReachAlgorithm(), depth(depth){};


    ~PartitionedDAPReachAlgorithmImplementation() override = default;

private:

    const unsigned depth = 0U;

    DynamicAPReachAlgorithm *createOverlayAlgorithm() override {
        return new OverlayAlgorithm;
    }

    DynamicAPReachAlgorithm *createSubAlgorithm() override {
        if( depth > 0U){
            auto* newAlgo = new PartitionedDAPReachAlgorithmImplementation<SubAlgorithm, OverlayAlgorithm>(depth - 1);
            if(propagatePartitionFunction){
                newAlgo->setPartitionFunction(partitionFunction, k);
            }
            return newAlgo;
        } else{
            return new SubAlgorithm;
        }
    }
};
#endif //ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H
