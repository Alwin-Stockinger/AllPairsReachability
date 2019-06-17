//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H
#define ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H

#include <property/propertymap.h>
#include "property/fastpropertymap.h"

#include "DynamicAPReachAlgorithm.h"

class PartitionedDAPReachAlgorithm : public DynamicAPReachAlgorithm{
public:

    ~PartitionedDAPReachAlgorithm() override;

    void run() override;

    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;

    bool query(const Algora::Vertex *start, const Algora::Vertex *end) override;

    std::string const getBaseName() override {
        return overlayAlgorithm->getBaseName();
    }

    void partition (const Algora::FastPropertyMap<unsigned long long int> &partitionMap,
                    unsigned long long int k);

    void onArcAdd(Algora::Arc *arc) override;

    void onArcRemove(Algora::Arc *arc) override;

    void onVertexAdd(Algora::Vertex *vertex) override;

    void onVertexRemove(Algora::Vertex *vertex) override;

    bool isPartitioned(){
        return partitioned;
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

    bool partitioned = false;

    void deleteOldPartition();

protected:
    virtual DynamicAPReachAlgorithm* createSubAlgorithm() = 0;
};


#endif //ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H
