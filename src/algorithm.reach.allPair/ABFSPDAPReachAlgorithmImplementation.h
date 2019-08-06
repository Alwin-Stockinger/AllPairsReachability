//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_ABFSPDAPREACHALGORITHMIMPLEMENTATION_H
#define ALLPAIRREACH_ABFSPDAPREACHALGORITHMIMPLEMENTATION_H

#include "AdvancedBFSOverlayPDAPReachAlgorithm.h"

template <typename SubAlgorithm, bool propagatePartitionFunction = true>
class ABFSPDAPReachAlgorithmImplementation : public AdvancedBFSOverlayPDAPReachAlgorithm{
public:
    static_assert(std::is_base_of<DynamicAPReachAlgorithm,SubAlgorithm>::value, "Template Parameter has to inherit from DynamicAPAlgorithm");

    explicit ABFSPDAPReachAlgorithmImplementation(const unsigned depth = 0U) : AdvancedBFSOverlayPDAPReachAlgorithm(){
        this->depth = depth;
    };


    ~ABFSPDAPReachAlgorithmImplementation() override = default;

private:

    DynamicAPReachAlgorithm *createSubAlgorithm() override {
        if( depth > 0U){
            auto* newAlgo = new ABFSPDAPReachAlgorithmImplementation<SubAlgorithm, propagatePartitionFunction>(depth - 1);
            if(propagatePartitionFunction){
                newAlgo->setPartitionFunction(partitionFunction, k);
            }
            return newAlgo;
        } else{
            return new SubAlgorithm;
        }
    }
};

#endif //ALLPAIRREACH_ABFSPDAPREACHALGORITHMIMPLEMENTATION_H
