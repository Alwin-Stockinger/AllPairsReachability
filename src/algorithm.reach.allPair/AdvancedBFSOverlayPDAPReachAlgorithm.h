//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_ADVANCEDBFSOVERLAYPDAPREACHALGORITHM_H
#define ALLPAIRREACH_ADVANCEDBFSOVERLAYPDAPREACHALGORITHM_H


#include "PartitionedDAPReachAlgorithm.h"

class AdvancedBFSOverlayPDAPReachAlgorithm : public PartitionedDAPReachAlgorithm{

public:
    bool query(Algora::Vertex *start, const Algora::Vertex *end) override;

    std::string getBaseName() override;

    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;
};


#endif //ALLPAIRREACH_ADVANCEDBFSOVERLAYPDAPREACHALGORITHM_H
