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

#include <iostream>
#include <chrono>
#include <numeric>
#include <fstream>
#include <map>
#include "AlgorithmTester.h"




struct AlgorithmTester::TimeCollector {


    explicit TimeCollector()= default;


    std::string algorithmName;
    std::string error ="";
    bool timedOut = false;


    unsigned long long initTime{};
    std::vector<unsigned long long> partitionTimes;
    std::vector<unsigned long long> queryTimes;
    std::vector<unsigned long long> addArcTimes;
    std::vector<unsigned long long> removeArcTimes;
    std::map<short, unsigned long long> percentageTimes;

    void addInitTime(TimePoint start, TimePoint end){
        initTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end -start).count();
    }

    void addPercentageTime(std::pair<short, unsigned long long> newTime) {
        percentageTimes.insert(newTime);
    }

    void addQueryTime(TimePoint start, TimePoint end) {
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        queryTimes.push_back(duration);
    }

    void addAddArcTime(TimePoint start, TimePoint end) {
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        addArcTimes.push_back(duration);
    }

    void addRemoveArcTime(TimePoint start, TimePoint end) {
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        removeArcTimes.push_back(duration);
    }

    void addPartitionTime(TimePoint start, TimePoint end) {
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        partitionTimes.push_back(duration);
    }

    double getAvgQueryTime(){
        return getAvg(queryTimes) - double(getRepartitionTime())/queryTimes.size();
    }

    double getAvgAddArcTime(){
        return getAvg(addArcTimes);
    }

    double getAvgRemoveArcTime(){
        return getAvg(removeArcTimes);
    }

    unsigned long long getRepartitionTime(){
        if(partitionTimes.size() > 1){
            return std::accumulate(++partitionTimes.begin(), partitionTimes.end(), 0ULL);
        }
        else{
            return 0ULL;
        }
    }

    unsigned long long getPartitionTime(){
        return std::accumulate(partitionTimes.begin(), partitionTimes.end(), 0ULL);
    }

    unsigned long long getQueryTime(){
        return std::accumulate(queryTimes.begin(), queryTimes.end(), 0ULL) - getRepartitionTime();
    }

    unsigned long long getAddArcTime(){
        return std::accumulate(addArcTimes.begin(), addArcTimes.end(), 0ULL);
    }

    unsigned long long getRemoveArcTime(){
        return std::accumulate(removeArcTimes.begin(), removeArcTimes.end(), 0ULL);
    }

    unsigned long long getAllTime(){
        return getQueryTime() + getAddArcTime() + getRemoveArcTime() + initTime + getRepartitionTime();
    }



private:
    static double getAvg(const std::vector<unsigned long long >& times){
        unsigned long long sum = std::accumulate(times.begin(), times.end(), 0ULL);

        auto avg = double(sum);
        return avg/times.size();
    }
};


void
AlgorithmTester::runTest(DynamicAPReachAlgorithm *algorithm, TimeCollector* timer) {

    auto* queries = instanceProvider->getQueries();
    auto* dynGraph = instanceProvider->getGraph();
    auto* diGraph = dynGraph->getDiGraph();

    TimePoint prevPartitionTime = partitionStartTimer;

    //measurement specific
    algorithm->setAutoUpdate(false);
    algorithm->setGraph(diGraph);

    std::cout << "Initializing next Algorithm " << std::endl;

    auto startTime = HRC::now();
    algorithm->run();
    auto endTime = HRC::now();
    timer->addInitTime(startTime, endTime);
    timer->addPartitionTime(partitionStartTimer, partitionEndTimer);
    prevPartitionTime = partitionStartTimer;

    std::cout << "Initialized in " << timer->initTime << "ns" << std::endl;
    std::cout << "Starting Algorithm " << algorithm->getBaseName() << std::endl;

    timer->algorithmName = algorithm->getBaseName();


    auto onArcAdded = [algorithm, timer](Algora::Arc* arc){
        auto startTime = HRC::now();
        algorithm->onArcAdd(arc);
        auto endTime = HRC::now();
        timer->addAddArcTime(startTime, endTime);
    };
    diGraph->onArcAdd(algorithm,onArcAdded);

    auto onArcRemoved = [algorithm, timer](Algora::Arc* arc){
        auto startTime = HRC::now();
        algorithm->onArcRemove(arc);
        auto endTime = HRC::now();
        timer->addRemoveArcTime(startTime, endTime);
    };
    diGraph->onArcRemove(algorithm, onArcRemoved);

    auto onVertexAdded = [algorithm, timer](Algora::Vertex* vertex){
        algorithm->onVertexAdd(vertex);
    };
    diGraph->onVertexAdd(algorithm, onVertexAdded);

    auto onVertexRemoved = [algorithm, timer](Algora::Vertex* vertex){
        algorithm->onVertexRemove(vertex);
    };
    diGraph->onVertexRemove(algorithm, onVertexRemoved);

    short progress = 0;
    unsigned long long currentStep = 0ULL;
    unsigned long long nextReport = 0ULL;
    unsigned long long reportStep = 1ULL;

    bool first = true;

    for (const auto &currentQueries : *queries) {

        if(first) first = false;
        else for (auto j = 0ULL; currentQueries.size() != 0ULL && j < currentQueries.size() - 1; j += 2) {

            auto startVertex = dynGraph->getCurrentVertexForId(currentQueries[j]);
            auto endVertex = dynGraph->getCurrentVertexForId(currentQueries[j + 1]);


            auto startQueryTime = std::chrono::high_resolution_clock::now();
            algorithm->query(startVertex, endVertex);
            auto endQueryTime = std::chrono::high_resolution_clock::now();
            timer->addQueryTime(startQueryTime, endQueryTime);
        }
        if(prevPartitionTime != partitionStartTimer){
            timer->addPartitionTime(partitionStartTimer, partitionEndTimer);
            prevPartitionTime = partitionStartTimer;
        }

        if( ++currentStep > nextReport){
            nextReport += reportStep;

            double progPercentage = double (currentStep) / queries->size();

            auto currentProg = short(progPercentage * 100.0);
            if(progress < currentProg){
                auto currentTime = timer->getAllTime();
                progress = currentProg;
                std::cout << progress << "% at " << currentTime << std::endl;
                timer->addPercentageTime(std::make_pair(progress, currentTime));
            }

            if(timeOut && timeOut < timer->getAllTime()){
                std::cout << "TIMEOUT" << std::endl;
                timer->timedOut = true;
                break;
            }
        }

        dynGraph->applyNextDelta();
    }

    algorithm->unsetGraph();

    diGraph->removeOnArcAdd(algorithm);
    diGraph->removeOnArcRemove(algorithm);
    diGraph->removeOnVertexAdd(algorithm);
    diGraph->removeOnVertexRemove(algorithm);





    std::cout << "Finished!\n\n" << std::endl;

}

void AlgorithmTester::runTests() {

    //writeHeader();

    for(DynamicAPReachAlgorithm* algorithm: algorithms){
        resetDynGraph();

        TimeCollector timer{};
        runTest(algorithm, &timer);

        writeResults(&timer);
        if(percentageTimes){
            writePercentageTimes(&timer);
        }
        delete algorithm;
    }

}

void AlgorithmTester::resetDynGraph() {

    Algora::DynamicDiGraph* dynGraph = instanceProvider->getGraph();

    //reset to initial graph
    dynGraph->resetToBigBang();
    dynGraph->applyNextDelta();

    //Fix for very strange bug that doesn't always happen
    dynGraph->resetToBigBang();
    dynGraph->applyNextDelta();
}

void AlgorithmTester::writeHeader() {

    std::ofstream file;

    file.open("results.csv", std::ios_base::app);

    file << instanceProvider->getConfiguration() << std::endl;

    file << "algorithm";
    file << ",avg query time(ns)";
    file << ",avg add Arc time(ns)";
    file << ",avg remove Arc time(ns)";
    file << ",init time(ns)";
    file << ",partition time(ns)";
    file << ",repartition time(ns)";
    file << ",whole Time(ns)";
    file << ",timeout";
    file << ",error";
    file << std::endl;
}

void AlgorithmTester::writeResults(AlgorithmTester::TimeCollector *timer) {
    std::ofstream file;
    file.open("results.csv", std::ios_base::app);

    file << "\"" << timer->algorithmName << "\"";
    file << "," << timer->getAvgQueryTime();
    file << "," << timer->getAvgAddArcTime();
    file << "," << timer->getAvgRemoveArcTime();
    file << "," << timer->initTime;
    file << "," << timer->getPartitionTime();
    file << "," << timer->getRepartitionTime();
    file << "," << timer->getAllTime();
    file << "," << timer->timedOut;
    file << "," << "\"" << timer->error << "\"";

    file << std::endl;
}

void AlgorithmTester::writePercentageTimes(AlgorithmTester::TimeCollector *timer){
    std::ofstream file;
    file.open("percentageTimes.csv", std::ios_base::app);

    file << "\"" << timer->algorithmName << "\"\n";
    file << "%,time\n"; //header


    for( auto& [percentage, time] : timer->percentageTimes){
        file << percentage << "," << time << "\n";
    }
    file << std::endl; //flush and mark end of algorithm
}

void AlgorithmTester::setPercentageTimes(bool b) {
    this->percentageTimes = b;
}
