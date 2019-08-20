#include <utility>

#include <utility>

//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H
#define ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H

#include <property/propertymap.h>
#include <unordered_set>
#include <random>
#include "property/fastpropertymap.h"

#include "DynamicAPReachAlgorithm.h"
#include "../partition/Partitioner.h"

using PartFunc = std::function<Algora::FastPropertyMap<unsigned long long>(unsigned long long int, Algora::DiGraph*)>;

class PartitionedAPReachAlgorithm : public DynamicAPReachAlgorithm{
public:

    std::mt19937 randomGenerator;
    std::uniform_int_distribution<unsigned long long> kDistribution;

    explicit PartitionedAPReachAlgorithm(unsigned long long seed = 38052ULL) : DynamicAPReachAlgorithm(){
        randomGenerator.seed(seed);
    }

    ~PartitionedAPReachAlgorithm() override;

    void run() override;

    void setK(unsigned long long newK){
        this->k = newK;

        initialized = false;
        kDistribution = std::uniform_int_distribution<unsigned long long >(0, k-1);
    }

    void onArcAdd(Algora::Arc *arc) override;

    void onArcRemove(Algora::Arc *arc) override;

    void onVertexAdd(Algora::Vertex *vertex) override;

    void onVertexRemove(Algora::Vertex *vertex) override;

    void setPartitionFunction(PartFunc newPartitionFunction,
                              unsigned long long newK){
        setK(newK);
        setPartitionFunction(std::move(newPartitionFunction));
    }
    void setPartitionFunction(PartFunc newPartitionFunction){
        partitionFunction = std::move(newPartitionFunction);

        initialized = false;
    }

    void setRepartitionThreshold(unsigned long long newThreshold){
        this->repartitionThreshold = newThreshold;
    }




protected:
    void onDiGraphSet() override;

    void onDiGraphUnset() override;

    virtual void insertOverlayEdgeArcs(Algora::DiGraph *subGraph) = 0;
    virtual void removeOverlayEdgeArcs(Algora::DiGraph *subGraph) = 0;

    virtual void resetChildStructures() = 0;
    virtual void initializeChildStructures() = 0;


    std::string getPartitionConfiguration(){
        return "k=" + std::to_string(k) + "/d=" + std::to_string(depth) + "/t=" + std::to_string(repartitionThreshold);
    }

    Algora::PropertyMap<std::unordered_set<Algora::Vertex*>> edgeVertices;
    //cannot use FastPropertyMap, because of different graphs???
    Algora::FastPropertyMap<Algora::Vertex*> mainToSubMap;
    Algora::FastPropertyMap<Algora::Vertex*> mainToOverlayMap;
    Algora::DiGraph* overlayGraph = nullptr;
    std::vector<Algora::DiGraph*> subGraphs;

private:



    void deleteOldPartition();



protected:
    std::vector<Algora::DiGraph*> partition ();

    PartFunc partitionFunction = Partitioner::handlePartitioning;

    unsigned long long k = 2;
    bool initialized = false;

    unsigned long long repartitionThreshold = 0;
    unsigned long long operationsSinceRepartition = 0;


private:

    void removeOverlayVertex(Algora::Vertex* vertex);

    bool checkIfOverlayIsolated(Algora::Vertex* vertex);

    virtual void insertOverlayEdgeArcsOfVertex(Algora::Vertex* vertex) = 0;


protected:
    unsigned depth = 0U;

    void addNeighbourVertex(Algora::Vertex *lazyVertex, Algora::Vertex *neighbourVertex);

    void addLazyVertexPair(Algora::Vertex *lazyHead, Algora::Vertex *lazyTail);

    virtual void addVertex(Algora::Vertex *lazyVertex, Algora::DiGraph *subGraph);
};


#endif //ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H
