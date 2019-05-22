//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H
#define ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H

#include <map>
#include <set>

#include "DynamicAPReachAlgorithm.h"
#include "SSBasedDAPReachAlgorithm.h"

template <typename T>
class PartitionedDAPReachAlgorithm : public DynamicAPReachAlgorithm{

public:
    static_assert(std::is_base_of<DynamicAPReachAlgorithm,T>::value, "Template Parameter has to inherit from DynamicApAlgorithm");

    explicit PartitionedDAPReachAlgorithm() : DynamicAPReachAlgorithm(){};

    explicit PartitionedDAPReachAlgorithm(std::vector<Algora::DiGraph *> &graphs, Algora::DiGraph *overlayGraph,
                                          std::map<const Algora::Vertex *, Algora::Vertex *> &inMap,
                                          std::map<const Algora::Vertex *, Algora::Vertex *> &mainToOverlayMap);

    ~PartitionedDAPReachAlgorithm() override;

    void run() override;

    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;

    bool query(const Algora::Vertex *start, const Algora::Vertex *end) override;

    void setGraphs(std::vector<Algora::DiGraph *>* graphs, Algora::DiGraph *overlayGraph, std::map<const Algora::Vertex*,Algora::Vertex*>& inMap, std::map<const Algora::Vertex*, Algora::Vertex*>& mainToOverlayMap);

protected:

private:
    void onVertexAdd(Algora::Vertex *vertex) override;

    void onVertexRemove(Algora::Vertex *vertex) override;

    void onArcAdd(Algora::Arc *arc) override;

    void onArcRemove(Algora::Arc *arc) override;

private:
    T* overlayAlgorithm = nullptr;

    std::set<T*> apAlgorithms;
    std::map<DynamicAPReachAlgorithm*, std::set<Algora::Vertex*>> edgeVertices;
    std::map<const Algora::Vertex*,Algora::Vertex*> inMap;
    std::map<const Algora::Vertex*,Algora::Vertex*> mainToOverlayMap;


    void deleteAlgorithms();
    T* findAlgorithm(const Algora::Vertex* vertex);

};


#endif //ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H
