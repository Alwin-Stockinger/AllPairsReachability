//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_STATICBFSAPREACHALGORITHM_H
#define ALLPAIRREACH_STATICBFSAPREACHALGORITHM_H


#include "SSBasedDAPReachAlgorithm.h"
#include <algorithm.reach/staticbfsssreachalgorithm.h>

class StaticBFSAPReachAlgorithm : public SSBasedDAPReachAlgorithm{

public:
    StaticBFSAPReachAlgorithm();

private:

    SSRAlgo *createAlgorithm() override;

    std::string getName() const noexcept override{
        return "All Pair Reachability Algorithm based on Static BFS Single-Source Reachability";
    }

    std::string getShortName() const noexcept override{
        return "AP based on Static-BFS-SSReach";
    }

};


#endif //ALLPAIRREACH_STATICBFSAPREACHALGORITHM_H
