//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_ALGORITHMTESTER_H
#define ALLPAIRREACH_ALGORITHMTESTER_H


#include <instanceprovider.h>


#include <chrono>
#include <utility>
#include "../algorithm.reach.allPair/DynamicAPReachAlgorithm.h"

typedef std::chrono::high_resolution_clock HRC;
typedef HRC::time_point TimePoint;

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

    std::chrono::high_resolution_clock::time_point& getPartitionStartTimer(){
        return partitionStartTimer;
    }

    std::chrono::high_resolution_clock::time_point& getPartitionEndTimer(){
        return partitionEndTimer;
    }

private:

    struct TimeCollector;

    void writeHeader();

    void writeResults(TimeCollector* timer);
    void resetDynGraph();
    void runTest(DynamicAPReachAlgorithm* algorithm, TimeCollector* timer);

    Algora::InstanceProvider* instanceProvider;
    std::vector<DynamicAPReachAlgorithm*> algorithms;
    unsigned long long timeOut;

    TimePoint partitionStartTimer;
    TimePoint partitionEndTimer;

};


#endif //ALLPAIRREACH_ALGORITHMTESTER_H
