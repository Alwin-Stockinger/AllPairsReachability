//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_SIMPLEDAPREACHALGORITHM_H
#define ALLPAIRREACH_SIMPLEDAPREACHALGORITHM_H


#include "DynamicAPReachAlgorithm.h"
#include "SSBasedDAPReachAlgorithm.h"


#include <property/fastpropertymap.h>
#include <type_traits>


class DynamicSSBasedDAPReachAlgorithm : public SSBasedDAPReachAlgorithm {

public:
    typedef Algora::FastPropertyMap<Algora::DynamicSSReachAlgorithm*> VertexAlgoMap;

    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;


    explicit DynamicSSBasedDAPReachAlgorithm() : SSBasedDAPReachAlgorithm(){ }
    ~DynamicSSBasedDAPReachAlgorithm() override;


    // DiGraphAlgorithm interface
    void run() override;


    // DynamicAPReachAlgorithm interface
    bool query(Algora::Vertex *start, const Algora::Vertex *end) override;

    std::string const getBaseName() override{
        auto* algorithm = createSSAlgorithm();
        std::string name = algorithm->getShortName();
        delete algorithm;
        return name;
    }

protected:

    // DynamicDiGraphAlgorithm interface
    void onVertexAdd(Algora::Vertex *vertex) override;
    void onVertexRemove(Algora::Vertex *vertex) override;


    // DiGraphAlgorithm interface
    void onDiGraphSet() override {
        DynamicAPReachAlgorithm::onDiGraphSet();
        init();
    };

    void onDiGraphUnset() override {
        DynamicAPReachAlgorithm::onDiGraphUnset();
        reset();
    }



private:

    VertexAlgoMap vertexMap;

    virtual void reset();

    virtual void init();

    virtual void addVertexToMap(Algora::Vertex *vertex);

    virtual void deleteAllAlgorithms();

};

#endif //ALLPAIRREACH_DYNAMICAPREACHALGORITHM_H
