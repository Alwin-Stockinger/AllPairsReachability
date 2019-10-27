//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_SIMPLEPARTITIONEDDAPREACHALGORITHM_H
#define ALLPAIRREACH_SIMPLEPARTITIONEDDAPREACHALGORITHM_H


#include "PartitionedAPReachAlgorithm.h"
#include "PartitionedDynamicAPReachAlgorithm.h"

class SimplePartitionedDAPReachAlgorithm : public PartitionedDynamicAPReachAlgorithm{

public:

    bool query(Algora::Vertex *start, const Algora::Vertex *end) override;

    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;

    std::string getBaseName() override  {

        std::string retName = "Simple";
        retName += "(" + getPartitionConfiguration() + ")";
        retName += ": " + graphToAlgorithmMap.begin()->second->getBaseName();

        retName += "(" + overlayAlgorithm->getBaseName() + ")";

        return retName;
    }

    ~SimplePartitionedDAPReachAlgorithm() override;

protected:
    virtual DynamicAPReachAlgorithm* createOverlayAlgorithm() = 0;

    void initializeChildStructures() override;

    void resetChildStructures() override;

private:
    DynamicAPReachAlgorithm* overlayAlgorithm = nullptr;
};


#endif //ALLPAIRREACH_SIMPLEPARTITIONEDDAPREACHALGORITHM_H
