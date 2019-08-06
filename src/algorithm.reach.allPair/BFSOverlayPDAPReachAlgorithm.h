//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_BFSOVERLAYPDAPREACHALGORITHM_H
#define ALLPAIRREACH_BFSOVERLAYPDAPREACHALGORITHM_H


#include "PartitionedAPReachAlgorithm.h"
#include "PartitionedDynamicAPReachAlgorithm.h"

class BFSOverlayPDAPReachAlgorithm : public PartitionedDynamicAPReachAlgorithm{

public:
    bool query(Algora::Vertex *start, const Algora::Vertex *end) override;

    std::string getBaseName() override;

    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;


    void setSetRemovals(bool b){
        setRemovals = b;
    }

private:
    bool setRemovals = false;

};


#endif //ALLPAIRREACH_BFSOVERLAYPDAPREACHALGORITHM_H
