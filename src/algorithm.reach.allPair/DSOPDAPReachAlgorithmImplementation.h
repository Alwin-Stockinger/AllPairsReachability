//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_DSOPDAPREACHALGORITHMIMPLEMENTATION_H
#define ALLPAIRREACH_DSOPDAPREACHALGORITHMIMPLEMENTATION_H

#include "DFSOverlayPDAPReachAlgorithm.h"

template <typename SubAlgorithm, bool propagatePartitionFunction = true>
class DFSOPDAPReachAlgorithmImplementation : public DFSOverlayPDAPReachAlgorithm{
public:
    static_assert(std::is_base_of<DynamicAPReachAlgorithm,SubAlgorithm>::value, "Template Parameter has to inherit from DynamicAPAlgorithm");

    explicit DFSOPDAPReachAlgorithmImplementation(const unsigned depth = 0U) : DFSOverlayPDAPReachAlgorithm(){
        this->depth = depth;
    };


    ~DFSOPDAPReachAlgorithmImplementation() override = default;

private:

    DynamicAPReachAlgorithm *createSubAlgorithm() override {
        if( depth > 0U){
            auto* newAlgo = new DFSOPDAPReachAlgorithmImplementation<SubAlgorithm, propagatePartitionFunction>(depth - 1);
            if(propagatePartitionFunction){
                newAlgo->setPartitionFunction(partitionFunction, k);
            }
            return newAlgo;
        } else{
            return new SubAlgorithm;
        }
    }
};


#endif //ALLPAIRREACH_DSOPDAPREACHALGORITHMIMPLEMENTATION_H
