//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_SIMPLEBFSOVERLAYPDAPREACHALGORITHM_H
#define ALLPAIRREACH_SIMPLEBFSOVERLAYPDAPREACHALGORITHM_H


#include "PartitionedDAPReachAlgorithm.h"

class SimpleBFSOverlayPDAPReachAlgorithm : public PartitionedDAPReachAlgorithm{
    std::string getName() const noexcept override;

    std::string getShortName() const noexcept  override;

    bool query(Algora::Vertex *start, const Algora::Vertex *end) override;

    std::string getBaseName() override;
};


#endif //ALLPAIRREACH_SIMPLEBFSOVERLAYPDAPREACHALGORITHM_H
