//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_ALGORITHMTESTER_H
#define ALLPAIRREACH_ALGORITHMTESTER_H


#include <instanceprovider.h>

#include <utility>
#include "../algorithm.reach.allPair/DynamicAPReachAlgorithm.h"

class AlgorithmTester {

public:

    void setAlgorithms(std::vector<DynamicAPReachAlgorithm*> newAlgorithms) {
        this->algorithms = std::move(newAlgorithms);
    }
    void setInstanceProvider(Algora::InstanceProvider* newInstanceProvider){
        this->instanceProvider = newInstanceProvider;
    }

    void setTimeOut( unsigned long long newTimeOut){
        this->timeOut = newTimeOut;
    }

    void runTests();

private:

    struct TimeCollector;

    void writeHeader();

    void writeResults(TimeCollector* timer);
    void resetDynGraph();
    void runTest(DynamicAPReachAlgorithm* algorithm, TimeCollector* timer);

    Algora::InstanceProvider* instanceProvider;
    std::vector<DynamicAPReachAlgorithm*> algorithms;
    unsigned long long timeOut;
};


#endif //ALLPAIRREACH_ALGORITHMTESTER_H
