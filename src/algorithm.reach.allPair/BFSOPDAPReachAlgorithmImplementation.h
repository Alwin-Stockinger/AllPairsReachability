//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_BFSOPDAPREACHALGORITHMIMPLEMENTATION_H
#define ALLPAIRREACH_BFSOPDAPREACHALGORITHMIMPLEMENTATION_H

#include "BFSOverlayPDAPReachAlgorithm.h"

template <typename SubAlgorithm, bool propagatePartitionFunction = true>
class BFSOPDAPReachAlgorithmImplementation : public BFSOverlayPDAPReachAlgorithm{
public:
    static_assert(std::is_base_of<DynamicAPReachAlgorithm,SubAlgorithm>::value, "Template Parameter has to inherit from DynamicAPAlgorithm");

    explicit BFSOPDAPReachAlgorithmImplementation(const unsigned depth = 0U) : BFSOverlayPDAPReachAlgorithm(){
        this->depth = depth;
    };


    ~BFSOPDAPReachAlgorithmImplementation() override = default;

private:

    DynamicAPReachAlgorithm *createSubAlgorithm() override {
        if( depth > 0U){
            auto* newAlgo = new BFSOPDAPReachAlgorithmImplementation<SubAlgorithm, propagatePartitionFunction>(depth - 1);
            if(propagatePartitionFunction){
                newAlgo->setPartitionFunction(partitionFunction, k);
            }
            return newAlgo;
        } else{
            return new SubAlgorithm;
        }
    }
};

#endif //ALLPAIRREACH_BFSOPDAPREACHALGORITHMIMPLEMENTATION_H
