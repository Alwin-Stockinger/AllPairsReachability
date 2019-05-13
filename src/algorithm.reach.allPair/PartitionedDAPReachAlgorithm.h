//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H
#define ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H

#include <map>

#include "DynamicAPReachAlgorithm.h"
#include "SSBasedDAPReachAlgorithm.h"

template <typename T>
class PartitionedDAPReachAlgorithm : DynamicAPReachAlgorithm{

public:
    static_assert(std::is_base_of<DynamicAPReachAlgorithm,T>::value, "Template Parameter has to inherit from DynamicApAlgorithm");

    explicit PartitionedDAPReachAlgorithm(std::vector<Algora::DiGraph*> graphs, Algora::DiGraph* connectorGraph);

    ~PartitionedDAPReachAlgorithm() override;

    void run() override;

    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;

    bool query(const Algora::Vertex *start, const Algora::Vertex *end) override;

    void setGraphs(std::vector<Algora::DiGraph*> graphs, Algora::DiGraph* connectorGraph);

protected:

private:
    void onVertexAdd(Algora::Vertex *vertex) override;

    void onVertexRemove(Algora::Vertex *vertex) override;

    void onArcAdd(Algora::Arc *arc) override;

    void onArcRemove(Algora::Arc *arc) override;

private:
    T* overlayAlgorithm = nullptr;

    std::vector<T*> apAlgorithms;
    std::map<DynamicAPReachAlgorithm*,std::vector<Algora::Vertex*>> edgeVertices;

    void deleteAlgorithms();

};


#endif //ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H
