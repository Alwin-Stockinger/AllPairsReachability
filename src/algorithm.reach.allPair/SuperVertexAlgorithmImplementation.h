//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_SUPERVERTEXALGORITHMIMPLEMENTATION_H
#define ALLPAIRREACH_SUPERVERTEXALGORITHMIMPLEMENTATION_H

#include "SuperVertexPDAPReachAlgorithm.h"

template <typename SubAlgorithm, typename OverlayAlgorithm, bool propagatePartitionFunction = true>
class SuperVertexAlgorithmImplementation : public SuperVertexPDAPReachAlgorithm{

        public:
        static_assert(std::is_base_of<DynamicAPReachAlgorithm,SubAlgorithm>::value, "Template Parameter has to inherit from DynamicAPAlgorithm");
        static_assert(std::is_base_of<Algora::DynamicSSReachAlgorithm,OverlayAlgorithm>::value, "Template Parameter has to inherit from DynamicSSReachAlgorithm");

        explicit SuperVertexAlgorithmImplementation(const unsigned depth = 0U) : SuperVertexPDAPReachAlgorithm(), depth(depth){};


        ~SuperVertexAlgorithmImplementation() override = default;

        private:

        const unsigned depth = 0U;

        Algora::DynamicSSReachAlgorithm *createOverlayAlgorithm() override {
                return new OverlayAlgorithm;
        }

        DynamicAPReachAlgorithm *createSubAlgorithm() override {
                if( depth > 0U){
                    auto* newAlgo = new SuperVertexAlgorithmImplementation<SubAlgorithm, OverlayAlgorithm, propagatePartitionFunction>(depth - 1);
                    if(propagatePartitionFunction){
                        newAlgo->setPartitionFunction(partitionFunction, k);
                    }
                    return newAlgo;
                } else{
                    return new SubAlgorithm;
                }
        }
    };


#endif //ALLPAIRREACH_SUPERVERTEXALGORITHMIMPLEMENTATION_H
