//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_DYNAMICAPREACHALGORITHM_H
#define ALLPAIRREACH_DYNAMICAPREACHALGORITHM_H


#include <algorithm.reach/dynamicssreachalgorithm.h>
#include <algorithm/dynamicdigraphalgorithm.h>

template <class derived>
class DynamicAPReachAlgorithm : public Algora::DynamicDiGraphAlgorithm{
public:

    static_assert(std::is_base_of<DynamicAPReachAlgorithm,derived>::value, "Template Parameter derived for sub algorithms has to inherit from DynamicApAlgorithm");

    explicit DynamicAPReachAlgorithm() : DynamicDiGraphAlgorithm(){ }
    ~DynamicAPReachAlgorithm() override = default;

    virtual bool query(const Algora::Vertex *start,const  Algora::Vertex *end) = 0;

    virtual std::string const getBaseName() = 0;

    static DynamicAPReachAlgorithm* createAlgorithm(){
        return derived::createAlgorithmImplementation();
    }
};


#endif //ALLPAIRREACH_DYNAMICAPREACHALGORITHM_H
