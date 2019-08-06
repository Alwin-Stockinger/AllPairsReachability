//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_SBFSPDAPREACHALGORITHMIMPLEMENTATION_H
#define ALLPAIRREACH_SBFSPDAPREACHALGORITHMIMPLEMENTATION_H

#include "SimpleBFSOverlayPDAPReachAlgorithm.h"

template <typename SubAlgorithm, bool propagatePartitionFunction = true>
class SBFSPDAPReachAlgorithmImplementation : public SimpleBFSOverlayPDAPReachAlgorithm{

public:
    static_assert(std::is_base_of<DynamicAPReachAlgorithm,SubAlgorithm>::value, "Template Parameter has to inherit from DynamicAPAlgorithm");

    explicit SBFSPDAPReachAlgorithmImplementation(const unsigned depth = 0U) : SimpleBFSOverlayPDAPReachAlgorithm(){
        this->depth = depth;
    };


    ~SBFSPDAPReachAlgorithmImplementation() override = default;

private:

    DynamicAPReachAlgorithm *createSubAlgorithm() override {
        if( depth > 0U){
            auto* newAlgo = new SBFSPDAPReachAlgorithmImplementation<SubAlgorithm, propagatePartitionFunction>(depth - 1);
            if(propagatePartitionFunction){
                newAlgo->setPartitionFunction(partitionFunction, k);
            }
            return newAlgo;
        } else{
            return new SubAlgorithm;
        }
    }

};


#endif //ALLPAIRREACH_SBFSPDAPREACHALGORITHMIMPLEMENTATION_H
