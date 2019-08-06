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

        std::string retName = "Simple";
        retName += "(k=" + std::to_string(k);
        retName += "/d=" + std::to_string(depth);
        retName += "): " + graphToAlgorithmMap.begin()->second->getBaseName();

        retName += "(" + overlayAlgorithm->getBaseName() + ")";

        return retName;
    }

    ~SimplePartitionedDAPReachAlgorithm() override;

protected:
    virtual DynamicAPReachAlgorithm* createOverlayAlgorithm() = 0;

private:
    DynamicAPReachAlgorithm* overlayAlgorithm = nullptr;
};


#endif //ALLPAIRREACH_SIMPLEPARTITIONEDDAPREACHALGORITHM_H
