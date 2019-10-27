//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_DFSOVERLAYPDAPREACHALGORITHM_H
#define ALLPAIRREACH_DFSOVERLAYPDAPREACHALGORITHM_H


#include "PartitionedDynamicAPReachAlgorithm.h"

class DFSOverlayPDAPReachAlgorithm : public PartitionedDynamicAPReachAlgorithm{


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


#endif //ALLPAIRREACH_DFSOVERLAYPDAPREACHALGORITHM_H
