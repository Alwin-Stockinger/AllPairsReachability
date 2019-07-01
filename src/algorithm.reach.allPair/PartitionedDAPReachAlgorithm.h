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
        return overlayAlgorithm->getBaseName();
    }

    void setK(unsigned long long newK){
        this->k = newK;
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



private:
    void initAlgorithms(std::vector<Algora::DiGraph *> &graphs);



protected:
    void onDiGraphSet() override;

    void onDiGraphUnset() override;


private:
    DynamicAPReachAlgorithm* overlayAlgorithm = nullptr;
    Algora::DiGraph* overlayGraph = nullptr;

    Algora::PropertyMap<std::set<Algora::Vertex*>> edgeVertices;//cannot use FastPropertyMap, because of different graphs???
    Algora::FastPropertyMap<Algora::Vertex*> mainToSubMap;
    Algora::FastPropertyMap<Algora::Vertex*> mainToOverlayMap;
    Algora::PropertyMap<DynamicAPReachAlgorithm*> graphToAlgorithmMap;//cannot use FastPropertyMap, because of different graphs???



    void deleteOldPartition();

    void partition ();

protected:
    virtual DynamicAPReachAlgorithm* createSubAlgorithm() = 0;

    PartFunc partitionFunction = Partitioner::handlePartitioning;

    unsigned long long k = 2;
    bool initialized = false;

    static unsigned long long graphIds;
};


#endif //ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H
