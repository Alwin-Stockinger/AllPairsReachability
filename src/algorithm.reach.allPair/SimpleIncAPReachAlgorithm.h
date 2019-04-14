//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_SIMPLEINCAPREACHALGORITHM_H
#define ALLPAIRREACH_SIMPLEINCAPREACHALGORITHM_H

#include <algorithm.reach/simpleincssreachalgorithm.h>
#include "SSBasedDAPReachAlgorithm.h"


class SimpleIncAPReachAlgorithm : SSBasedDAPReachAlgorithm{
public:

    explicit SimpleIncAPReachAlgorithm(bool reverse = false, bool searchForward = false, double maxUS= 1.0, bool radicalReset = false);

    void setMaxUnknownStateSqrt();
    void setMaxUnknownStateLog();
    void relateToReachableVertices( bool rTR);

    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;

private:
    SSRAlgo *createAlgorithm() override;


private:

    bool reverse;
    bool searchForward;
    double maxUS;
    bool radicalReset;

    bool relateToReachable;
    bool maxUSLog;
    bool maxUSSqrt;

};


#endif //ALLPAIRREACH_SIMPLEINCAPREACHALGORITHM_H
