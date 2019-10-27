//
// Copyright (c) 2019 : Alwin Stockinger
//
//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <https://www.gnu.org/licenses/>.
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

    void setPercentageTimes(bool b);

private:

    struct TimeCollector;

    void writeHeader();

    static void writeResults(TimeCollector* timer);
    static void writePercentageTimes(TimeCollector* timer);

    void resetDynGraph();
    void runTest(DynamicAPReachAlgorithm* algorithm, TimeCollector* timer);

    Algora::InstanceProvider* instanceProvider;
    std::vector<DynamicAPReachAlgorithm*> algorithms;
    unsigned long long timeOut;

    TimePoint partitionStartTimer;
    TimePoint partitionEndTimer;

    bool percentageTimes = false;

};


#endif //ALLPAIRREACH_ALGORITHMTESTER_H
