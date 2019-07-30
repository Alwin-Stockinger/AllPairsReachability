//
// Created by alwin on 7/22/19.
//

#ifndef ALLPAIRREACH_SUPERVERTEXPDAPREACHALGORITHM_H
#define ALLPAIRREACH_SUPERVERTEXPDAPREACHALGORITHM_H


#include "PartitionedDAPReachAlgorithm.h"
#include "SimplePartitionedDAPReachAlgorithmImplementation.h"


class SuperVertexPDAPReachAlgorithm
        : public PartitionedDAPReachAlgorithm {

public:
    void run() override {
        if(initialized){
            return;
        }

        resetSuperStructure();

        partition();

        for(const auto &[_,algorithm] : graphToAlgorithmMap){
            algorithm -> run();

            (void)(_);  //unused warning
        }

        generateSuperVertices();
        generateOverlayAlgorithms();

        registerOnOverlay();

        initialized = true;
    }

    bool query(Algora::Vertex *start, const Algora::Vertex *end) override;


    ~SuperVertexPDAPReachAlgorithm() override;

    Algora::PropertyMap<Algora::Vertex*> sourceSuperVertices;
    Algora::PropertyMap<Algora::Vertex*> destinationSuperVertices;
    Algora::PropertyMap<Algora::DynamicSSReachAlgorithm*> overlayAlgorithms;

    void onArcAdd(Algora::Arc *arc) override;

    const std::string getBaseName() override;

    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;

protected:
    virtual Algora::DynamicSSReachAlgorithm* createOverlayAlgorithm() = 0;

private:

    void registerOnOverlay();
    void deregisterOnOverlay();

    void generateSuperVertices();
    void generateOverlayAlgorithms();
    void resetSuperStructure();

    std::vector<Algora::Vertex*> delayedVertices;

};


#endif //ALLPAIRREACH_SUPERVERTEXPDAPREACHALGORITHM_H
