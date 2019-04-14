//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_SIMPLEDAPREACHALGORITHM_H
#define ALLPAIRREACH_SIMPLEDAPREACHALGORITHM_H


#include "DynamicAPReachAlgorithm.h"


#include <property/fastpropertymap.h>

#include <memory>

class SSBasedDAPReachAlgorithm : public DynamicAPReachAlgorithm{



public:
    typedef Algora::DynamicSSReachAlgorithm SSRAlgo;
    typedef Algora::FastPropertyMap<std::unique_ptr<Algora::DynamicSSReachAlgorithm>> VertexAlgoMap;


    explicit SSBasedDAPReachAlgorithm() : DynamicAPReachAlgorithm(){ }
    ~SSBasedDAPReachAlgorithm() override = default;


    // DiGraphAlgorithm interface
    void run() override;


    // DynamicAPReachAlgorithm interface
    bool query(const Algora::Vertex *start, const Algora::Vertex *end) override;

protected:

    // DynamicDiGraphAlgorithm interface
    void onVertexAdd(Algora::Vertex *vertex) override;
    void onVertexRemove(Algora::Vertex *vertex) override;


    // DiGraphAlgorithm interface
    void onDiGraphSet() override {
        DynamicDiGraphAlgorithm::onDiGraphSet();
        reset();
    };

    void onDiGraphUnset() override {
        DynamicDiGraphAlgorithm::onDiGraphUnset();
        reset();
    }

    //Factory Method for specific Algorithm creation
    virtual SSRAlgo* createAlgorithm() = 0;

private:

    VertexAlgoMap vertexMap;

    virtual void reset(){
        vertexMap.resetAll();
        if(diGraph) init();
    }

    virtual void init();

    virtual void addVertexToMap(Algora::Vertex *vertex);

};




#endif //ALLPAIRREACH_DYNAMICAPREACHALGORITHM_H
