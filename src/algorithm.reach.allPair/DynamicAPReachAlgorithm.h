//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_DYNAMICAPREACHALGORITHM_H
#define ALLPAIRREACH_DYNAMICAPREACHALGORITHM_H


#include <algorithm.reach/dynamicssreachalgorithm.h>
#include <algorithm/dynamicdigraphalgorithm.h>

class DynamicAPReachAlgorithm : public Algora::DynamicDiGraphAlgorithm{
public:

    explicit DynamicAPReachAlgorithm() : DynamicDiGraphAlgorithm(){ }
    ~DynamicAPReachAlgorithm() override = default;

    virtual bool query(const Algora::Vertex *start,const  Algora::Vertex *end) = 0;

    bool containsVertex(const Algora::Vertex* vertex);

    Algora::Arc* addArc(Algora::Vertex* head, Algora::Vertex* tail );

    void removeArc(Algora::Vertex* head, Algora::Vertex* tail);

    bool isIsolated(const Algora::Vertex* vertex);

};


#endif //ALLPAIRREACH_DYNAMICAPREACHALGORITHM_H
