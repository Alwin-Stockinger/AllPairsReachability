//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_SIMPLEPARTITIONEDDAPREACHALGORITHM_H
#define ALLPAIRREACH_SIMPLEPARTITIONEDDAPREACHALGORITHM_H


#include "PartitionedDAPReachAlgorithm.h"

class SimplePartitionedDAPReachAlgorithm : public PartitionedDAPReachAlgorithm{

public:
    void run() override;

    bool query(Algora::Vertex *start, const Algora::Vertex *end) override;

    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;

    std::string getBaseName() override  {

        std::string subName = graphToAlgorithmMap.begin()->second->getBaseName();
        std::string overlayName = overlayAlgorithm->getBaseName();

        return subName + " (" + overlayName + ")";


    }

    ~SimplePartitionedDAPReachAlgorithm() override;

protected:
    virtual DynamicAPReachAlgorithm* createOverlayAlgorithm() = 0;

private:
    DynamicAPReachAlgorithm* overlayAlgorithm = nullptr;
};


#endif //ALLPAIRREACH_SIMPLEPARTITIONEDDAPREACHALGORITHM_H
