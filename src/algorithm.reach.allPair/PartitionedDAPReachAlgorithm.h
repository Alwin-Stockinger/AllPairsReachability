//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H
#define ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H


#include "DynamicAPReachAlgorithm.h"
#include "SSBasedDAPReachAlgorithm.h"

template <typename T>
class PartitionedDAPReachAlgorithm : DynamicAPReachAlgorithm{

    static_assert(std::is_base_of<DynamicAPReachAlgorithm,T>::value, "Template Parameter has to inherit from DynamicApAlgorithm");

    explicit PartitionedDAPReachAlgorithm(std::vector<Algora::DiGraph*>* graphs, Algora::DiGraph* connectorGraph);

    ~PartitionedDAPReachAlgorithm() override;

    void run() override;

    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;

    bool query(const Algora::Vertex *start, const Algora::Vertex *end) override;

    void setGraphs(std::vector<Algora::DiGraph*>* graphs, Algora::DiGraph* connectorGraph);


private:
    T overlayAlgorithm;

    std::vector<T*> apAlgorithms;
    Algora::FastPropertyMap<std::vector<Algora::Vertex*>> edgeVertices;

    void deleteAlgorithms();

};


#endif //ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H
