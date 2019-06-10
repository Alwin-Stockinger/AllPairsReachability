//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHMIMPLEMENTATION_H
#define ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHMIMPLEMENTATION_H

#include <map>
#include <set>
#include <property/propertymap.h>

#include "DynamicAPReachAlgorithm.h"
#include "SSBasedDAPReachAlgorithm.h"
#include "PartitionedDAPReachAlgorithm.h"

template <typename T>
class PartitionedDAPReachAlgorithmImplementation : public PartitionedDAPReachAlgorithm{

public:
    static_assert(std::is_base_of<DynamicAPReachAlgorithm,T>::value, "Template Parameter has to inherit from DynamicApAlgorithm");

    explicit PartitionedDAPReachAlgorithmImplementation() : PartitionedDAPReachAlgorithm(){};

    ~PartitionedDAPReachAlgorithmImplementation() override;

    void run() override;

    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;

    bool query(const Algora::Vertex *start, const Algora::Vertex *end) override;

    std::string const getBaseName() override {
        return overlayAlgorithm->getBaseName();
    }

    void partition (const Algora::FastPropertyMap<unsigned long long int> &partitionMap,
                   unsigned long long int k) override;

    void onArcAdd(Algora::Arc *arc) override;

    void onArcRemove(Algora::Arc *arc) override;

    void onVertexAdd(Algora::Vertex *vertex) override;

    void onVertexRemove(Algora::Vertex *vertex) override;

private:
    void initAlgorithms(std::vector<Algora::DiGraph *> &graphs, Algora::DiGraph *overlayGraph);


protected:
    void onDiGraphSet() override;

    void onDiGraphUnset() override;


private:
    T* overlayAlgorithm = nullptr;

    Algora::PropertyMap<std::set<Algora::Vertex*>> edgeVertices;//cannot use FastPropertyMap, because of different graphs???
    Algora::FastPropertyMap<Algora::Vertex*> inMap;
    Algora::FastPropertyMap<Algora::Vertex*> mainToOverlayMap;
    Algora::PropertyMap<DynamicAPReachAlgorithm*> graphToAlgorithmMap;//cannot use FastPropertyMap, because of different graphs???

    bool partitioned = false;

    void deleteOldPartition();


    void initEdges(const std::vector<Algora::DiGraph *> &graphs, Algora::DiGraph *overlayGraph);
};


#endif //ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHMIMPLEMENTATION_H
