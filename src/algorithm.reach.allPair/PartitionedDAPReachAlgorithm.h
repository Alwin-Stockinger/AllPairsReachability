#include <utility>

#include <utility>

//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H
#define ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H

#include <property/propertymap.h>
#include "property/fastpropertymap.h"

#include "DynamicAPReachAlgorithm.h"
#include "../partition/Partitioner.h"

using PartFunc = std::function<Algora::FastPropertyMap<unsigned long long>(unsigned long long int, Algora::DiGraph*)>;

class PartitionedDAPReachAlgorithm : public DynamicAPReachAlgorithm{
public:

    ~PartitionedDAPReachAlgorithm() override;

    void run() override;

    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;

    bool query(Algora::Vertex *start, const Algora::Vertex *end) override;

    std::string const getBaseName() override {

        std::string subName = graphToAlgorithmMap.begin()->second->getBaseName();
        std::string overlayName = overlayAlgorithm->getBaseName();

        return subName + " (" + overlayName + ")";
    }

    void setK(unsigned long long newK){
        this->k = newK;

        initialized = false;
    }

    void onArcAdd(Algora::Arc *arc) override;

    void onArcRemove(Algora::Arc *arc) override;

    void onVertexAdd(Algora::Vertex *vertex) override;

    void onVertexRemove(Algora::Vertex *vertex) override;

    void setPartitionFunction(PartFunc newPartitionFunction,
                              unsigned long long newK){
        k = newK;
        partitionFunction = std::move(newPartitionFunction);
    }
    void setPartitionFunction(PartFunc newPartitionFunction){
        partitionFunction = std::move(newPartitionFunction);
    }

    void setRepartitionThreshold(unsigned long long newThreshold){
        this->repartitionThreshold = newThreshold;
    }

    void insertOverlayEdgeArcs(Algora::DiGraph *subGraph);
    void removeOverlayEdgeArcs(Algora::DiGraph *subGraph);


private:
    void initAlgorithms(std::vector<Algora::DiGraph *> &graphs);



protected:
    void onDiGraphSet() override;

    void onDiGraphUnset() override;


    Algora::FastPropertyMap<Algora::Vertex*> mainToSubMap;
    Algora::PropertyMap<DynamicAPReachAlgorithm*> graphToAlgorithmMap;
    Algora::PropertyMap<std::set<Algora::Vertex*>> edgeVertices;
//cannot use FastPropertyMap, because of different graphs???
Algora::FastPropertyMap<Algora::Vertex*> mainToOverlayMap;
    Algora::DiGraph* overlayGraph = nullptr;
    DynamicAPReachAlgorithm* overlayAlgorithm = nullptr;
private:



    void deleteOldPartition();

    void partition ();

protected:
    virtual DynamicAPReachAlgorithm* createSubAlgorithm() = 0;
    virtual DynamicAPReachAlgorithm* createOverlayAlgorithm() = 0;

    PartFunc partitionFunction = Partitioner::handlePartitioning;

    unsigned long long k = 2;
    bool initialized = false;

    unsigned long long repartitionThreshold = 0;
    unsigned long long operationsSinceRepartition = 0;
};


#endif //ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H
