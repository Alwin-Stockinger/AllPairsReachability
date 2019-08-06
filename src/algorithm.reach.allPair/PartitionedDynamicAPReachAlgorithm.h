//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_PARTITIONEDDYNAMICAPREACHALGORITHM_H
#define ALLPAIRREACH_PARTITIONEDDYNAMICAPREACHALGORITHM_H

#include "PartitionedAPReachAlgorithm.h"

#include "DynamicAPReachAlgorithm.h"

class PartitionedDynamicAPReachAlgorithm : public PartitionedAPReachAlgorithm{

public:

    ~PartitionedDynamicAPReachAlgorithm() override{
        for(auto &[_,algorithm] : graphToAlgorithmMap){
            delete(algorithm);
        }
    }

protected:

    virtual DynamicAPReachAlgorithm* createSubAlgorithm() = 0;

    void insertOverlayEdgeArcsOfVertex(Algora::Vertex* vertex) override;

    void insertOverlayEdgeArcs(Algora::DiGraph *subGraph) override;

    void removeOverlayEdgeArcs(Algora::DiGraph *subGraph) override;

    Algora::PropertyMap<DynamicAPReachAlgorithm*> graphToAlgorithmMap;

    void initializeChildStructures() override;
    void resetChildStructures() override ;

private:

};


#endif //ALLPAIRREACH_PARTITIONEDDYNAMICAPREACHALGORITHM_H
