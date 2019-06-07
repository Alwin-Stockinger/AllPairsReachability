//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H
#define ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H

#include <map>
#include <set>
#include <property/propertymap.h>

#include "DynamicAPReachAlgorithm.h"
#include "SSBasedDAPReachAlgorithm.h"

template <typename T>
class PartitionedDAPReachAlgorithm : public DynamicAPReachAlgorithm{

public:
    static_assert(std::is_base_of<DynamicAPReachAlgorithm,T>::value, "Template Parameter has to inherit from DynamicApAlgorithm");

    explicit PartitionedDAPReachAlgorithm() : DynamicAPReachAlgorithm(){};

    explicit PartitionedDAPReachAlgorithm(std::vector<Algora::DiGraph *> &graphs, Algora::DiGraph *overlayGraph,
                                          Algora::FastPropertyMap< Algora::Vertex*>& inMap, Algora::FastPropertyMap< Algora::Vertex*>& mainToOverlayMap);

    ~PartitionedDAPReachAlgorithm() override;

    void run() override;

    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;

    bool query(const Algora::Vertex *start, const Algora::Vertex *end) override;

    void setGraphs(std::vector<Algora::DiGraph *>* graphs, Algora::DiGraph *overlayGraph, Algora::FastPropertyMap< Algora::Vertex*>& inMap, Algora::FastPropertyMap< Algora::Vertex*>& mainToOverlayMap);

protected:

private:
    void onVertexAdd(Algora::Vertex *vertex) override;

    void onVertexRemove(Algora::Vertex *vertex) override;

    void onArcAdd(Algora::Arc *arc) override;

    void onArcRemove(Algora::Arc *arc) override;

private:
    T* overlayAlgorithm = nullptr;

    Algora::PropertyMap<std::set<Algora::Vertex*>> edgeVertices;
    Algora::FastPropertyMap<Algora::Vertex*> inMap;
    Algora::FastPropertyMap<Algora::Vertex*> mainToOverlayMap;
    Algora::PropertyMap<DynamicAPReachAlgorithm*> graphToAlgorithmMap;//can not use FastPropertyMap, because of different graphs???


    void deleteAlgorithms();

};


#endif //ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H
