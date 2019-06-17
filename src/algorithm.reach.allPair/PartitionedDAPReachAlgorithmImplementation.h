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

template <class T, class S = T>
class PartitionedDAPReachAlgorithmImplementation : public PartitionedDAPReachAlgorithm<PartitionedDAPReachAlgorithmImplementation<T,S>>{

public:
    static_assert(std::is_base_of<DynamicAPReachAlgorithm<T>,T>::value, "Template Parameter for sub algorithms has to inherit from DynamicApAlgorithm");
    static_assert(std::is_base_of<DynamicAPReachAlgorithm<S>,S>::value, "Template Parameter for overlay algorithmhas to inherit from DynamicApAlgorithm");

    explicit PartitionedDAPReachAlgorithmImplementation() : PartitionedDAPReachAlgorithm<PartitionedDAPReachAlgorithmImplementation<T,S>>(){};

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

    bool isPartitioned(){
        return partitioned;
    }

    static DynamicAPReachAlgorithm<T>* createAlgorithmImplementation(){
        return new PartitionedDAPReachAlgorithmImplementation<T,S>();
    }

private:
    void initAlgorithms(std::vector<Algora::DiGraph *> &graphs);


protected:
    void onDiGraphSet() override;

    void onDiGraphUnset() override;


private:
    DynamicAPReachAlgorithm<S>* overlayAlgorithm = nullptr;
    Algora::DiGraph* overlayGraph = nullptr;

    Algora::PropertyMap<std::set<Algora::Vertex*>> edgeVertices;//cannot use FastPropertyMap, because of different graphs???
    Algora::FastPropertyMap<Algora::Vertex*> mainToSubMap;
    Algora::FastPropertyMap<Algora::Vertex*> mainToOverlayMap;
    Algora::PropertyMap<DynamicAPReachAlgorithm<T>*> graphToAlgorithmMap;//cannot use FastPropertyMap, because of different graphs???

    bool partitioned = false;

    void deleteOldPartition();
};


#endif //ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHMIMPLEMENTATION_H
