//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_SIMPLEDAPREACHALGORITHM_H
#define ALLPAIRREACH_SIMPLEDAPREACHALGORITHM_H


#include "DynamicAPReachAlgorithm.h"
#include "PartitionedDAPReachAlgorithm.h"


#include <property/fastpropertymap.h>

#include <memory>
#include <type_traits>

template <typename T>
class SSBasedDAPReachAlgorithm : public PartitionedDAPReachAlgorithm<SSBasedDAPReachAlgorithm<T>> {

    static_assert(std::is_base_of<Algora::DynamicSSReachAlgorithm,T>::value, "Template Parameter has to inherit from Algora::DynamicSSReachAlgorithm");

public:
    typedef Algora::FastPropertyMap<Algora::DynamicSSReachAlgorithm*> VertexAlgoMap;

    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;

    explicit SSBasedDAPReachAlgorithm() : DynamicAPReachAlgorithm<SSBasedDAPReachAlgorithm<T>>(){ }
    ~SSBasedDAPReachAlgorithm() override;


    // DiGraphAlgorithm interface
    void run() override;


    // DynamicAPReachAlgorithm interface
    bool query(const Algora::Vertex *start, const Algora::Vertex *end) override;

    std::string const getBaseName() override{
        return vertexMap.getValue(this->diGraph->getAnyVertex())->getShortName();
    }

protected:

    // DynamicDiGraphAlgorithm interface
    void onVertexAdd(Algora::Vertex *vertex) override;
    void onVertexRemove(Algora::Vertex *vertex) override;


    // DiGraphAlgorithm interface
    void onDiGraphSet() override {
        DynamicAPReachAlgorithm<SSBasedDAPReachAlgorithm<T>>::onDiGraphSet();
        init();
    };

    void onDiGraphUnset() override {
        DynamicAPReachAlgorithm<SSBasedDAPReachAlgorithm<T>>::onDiGraphUnset();
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
