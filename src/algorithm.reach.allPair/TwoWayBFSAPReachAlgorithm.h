//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_TWOWAYBFSAPREACHALGORITHM_H
#define ALLPAIRREACH_TWOWAYBFSAPREACHALGORITHM_H


#include "DynamicAPReachAlgorithm.h"

class TwoWayBFSAPReachAlgorithm : public DynamicAPReachAlgorithm{
public:
    bool query(Algora::Vertex *start, const Algora::Vertex *end) override;

    std::string getBaseName() override;

    void setStepSize(unsigned long long newStep){
        stepSize = newStep;
    }

private:
public:
    void run() override;

    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;

private:
    unsigned long long stepSize = 1ULL;

};



#endif //ALLPAIRREACH_TWOWAYBFSAPREACHALGORITHM_H
