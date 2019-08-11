//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_REVERSEBFSPARTITIONEDAPREACHALGORITHM_H
#define ALLPAIRREACH_REVERSEBFSPARTITIONEDAPREACHALGORITHM_H


#include <numeric>
#include "PartitionedAPReachAlgorithm.h"

class ReverseBFSPartitionedAPReachAlgorithm : public PartitionedAPReachAlgorithm{

public:
    bool query(Algora::Vertex *start, const Algora::Vertex *end) override;


    void setSetRemovals( bool b){
        this->setRemovals = b;
    }

    std::string getBaseName() override;

protected:

    void initializeChildStructures() override;

    void insertOverlayEdgeArcs(Algora::DiGraph *subGraph) override;

    void insertOverlayEdgeArcsOfVertex(Algora::Vertex *vertex) override;

    void removeOverlayEdgeArcs(Algora::DiGraph *subGraph) override;

    void resetChildStructures() override;

public:
    void onVertexAdd(Algora::Vertex *vertex) override;

    void onVertexRemove(Algora::Vertex *vertex) override;

public:
    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;

    unsigned long long getAmount(){
        unsigned long long sum = 0ULL;

        for(auto& [_,edgeSet] : edgeVertices){
            sum += edgeSet.size();
        }
        return sum;
    }

private:
    Algora::PropertyMap<Algora::Vertex*> subToMainMap; //can't use FastPropertyMap because vertices of subgraphs could have same id

    bool setRemovals = true;
};


#endif //ALLPAIRREACH_REVERSEBFSPARTITIONEDAPREACHALGORITHM_H
