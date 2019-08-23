//
// Created by alwin on 7/22/19.
//

#ifndef ALLPAIRREACH_BIDIRECTIONALSUPERVERTEXPDAPREACHALGORITHM_H
#define ALLPAIRREACH_BIDIRECTIONALSUPERVERTEXPDAPREACHALGORITHM_H


#include "PartitionedAPReachAlgorithm.h"
#include "SimplePartitionedDAPReachAlgorithmImplementation.h"


class BidirectionalSuperVertexPDAPReachAlgorithm
        : public PartitionedDynamicAPReachAlgorithm {

public:


    bool query(Algora::Vertex *start, const Algora::Vertex *end) override;


    ~BidirectionalSuperVertexPDAPReachAlgorithm() override;

    Algora::PropertyMap<Algora::Vertex*> sourceSuperVertices;
    Algora::PropertyMap<Algora::Vertex*> destinationSuperVertices;

    void onArcAdd(Algora::Arc *arc) override;

    std::string getBaseName() override;

    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;

protected:
    virtual Algora::DynamicSSReachAlgorithm* createOverlayAlgorithm() = 0;

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


#endif //ALLPAIRREACH_BIDIRECTIONALSUPERVERTEXPDAPREACHALGORITHM_H
