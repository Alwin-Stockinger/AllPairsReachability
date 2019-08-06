//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_SIMPLEPARTITIONEDDAPREACHALGORITHMIMPLEMENTATION_H
#define ALLPAIRREACH_SIMPLEPARTITIONEDDAPREACHALGORITHMIMPLEMENTATION_H

#include <map>
#include <set>
#include <property/propertymap.h>

#include "DynamicAPReachAlgorithm.h"
#include "DynamicSSBasedDAPReachAlgorithm.h"
#include "PartitionedDAPReachAlgorithm.h"
#include "SimplePartitionedDAPReachAlgorithm.h"

template <typename SubAlgorithm, typename OverlayAlgorithm = SubAlgorithm, bool propagatePartitionFunction = true>
class SimplePartitionedDAPReachAlgorithmImplementation : public SimplePartitionedDAPReachAlgorithm{

public:
    static_assert(std::is_base_of<DynamicAPReachAlgorithm,SubAlgorithm>::value, "Template Parameter has to inherit from DynamicAPAlgorithm");
    static_assert(std::is_base_of<DynamicAPReachAlgorithm,OverlayAlgorithm>::value, "Template Parameter has to inherit from DynamicAPAlgorithm");

    explicit SimplePartitionedDAPReachAlgorithmImplementation(const unsigned depth = 0U) : SimplePartitionedDAPReachAlgorithm(){
        this->depth=depth;
    };


    ~SimplePartitionedDAPReachAlgorithmImplementation() override = default;

private:

    DynamicAPReachAlgorithm *createOverlayAlgorithm() override {
        return new OverlayAlgorithm;
    }

    DynamicAPReachAlgorithm *createSubAlgorithm() override {
        if( depth > 0U){
            auto* newAlgo = new SimplePartitionedDAPReachAlgorithmImplementation<SubAlgorithm, OverlayAlgorithm, propagatePartitionFunction>(depth - 1);
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
