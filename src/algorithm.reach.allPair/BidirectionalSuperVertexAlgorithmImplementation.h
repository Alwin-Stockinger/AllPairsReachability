//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_BIDIRECTIONALSUPERVERTEXALGORITHMIMPLEMENTATION_H
#define ALLPAIRREACH_BIDIRECTIONALSUPERVERTEXALGORITHMIMPLEMENTATION_H

#include "DynamicAPReachAlgorithm.h"
#include "BidirectionalSuperVertexAlgorithm.h"

template <typename SubAlgorithm, bool propagatePartitionFunction = true>
class BidirectionalSuperVertexAlgorithmImplementation : public BidirectionalSuperVertexAlgorithm{

public:
    static_assert(std::is_base_of<DynamicAPReachAlgorithm,SubAlgorithm>::value, "Template Parameter has to inherit from DynamicAPAlgorithm");

    explicit BidirectionalSuperVertexAlgorithmImplementation(const unsigned depth = 0U) : BidirectionalSuperVertexAlgorithm(){
        this->depth = depth;
    };


    ~BidirectionalSuperVertexAlgorithmImplementation() override = default;

private:

    DynamicAPReachAlgorithm *createSubAlgorithm() override {
        if( depth > 0U){
            auto* newAlgo = new BidirectionalSuperVertexAlgorithmImplementation<SubAlgorithm,propagatePartitionFunction>(depth - 1);
            if(propagatePartitionFunction){
                newAlgo->setPartitionFunction(partitionFunction, k);
            }
            return newAlgo;
        } else{
            return new SubAlgorithm;
        }
    }

};


#endif //ALLPAIRREACH_BIDIRECTIONALSUPERVERTEXALGORITHMIMPLEMENTATION_H
