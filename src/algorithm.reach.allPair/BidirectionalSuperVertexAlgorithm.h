//
// Created by alwin on 7/22/19.
//

#ifndef ALLPAIRREACH_BIDIRECTIONALSUPERVERTEXALGORITHM_H
#define ALLPAIRREACH_BIDIRECTIONALSUPERVERTEXALGORITHM_H


#include "PartitionedAPReachAlgorithm.h"
#include "SimplePartitionedDAPReachAlgorithmImplementation.h"


class BidirectionalSuperVertexAlgorithm
        : public PartitionedDynamicAPReachAlgorithm {

public:


    bool query(Algora::Vertex *start, const Algora::Vertex *end) override;


    ~BidirectionalSuperVertexAlgorithm() override;

    Algora::PropertyMap<Algora::Vertex*> sourceSuperVertices;
    Algora::PropertyMap<Algora::Vertex*> destinationSuperVertices;

    void onArcAdd(Algora::Arc *arc) override;

    std::string getBaseName() override;

    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;

protected:

    void initializeChildStructures() override;

    void resetChildStructures() override;

private:

    void registerOnOverlay();
    void deregisterOnOverlay();

    void generateSuperVertices();
    void resetSuperStructure();

    std::vector<Algora::Vertex*> delayedVertices;

    unsigned long long stepSize = 1ULL;

    bool bidirectionalOverlaySearch(Algora::Vertex* source, Algora::Vertex* target);

};


#endif //ALLPAIRREACH_BIDIRECTIONALSUPERVERTEXALGORITHM_H
