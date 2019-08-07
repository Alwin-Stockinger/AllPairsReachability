//
// Created by Alwin Stockinger.
//

#include <iostream>
#include <chrono>
#include <numeric>
#include <fstream>
#include "AlgorithmTester.h"

typedef std::chrono::high_resolution_clock HRC;
typedef HRC::time_point TimePoint;


struct AlgorithmTester::TimeCollector {


    explicit TimeCollector()= default;


    std::string algorithmName;
    std::string error ="";
    bool timedOut = false;


    unsigned long long initTime{};
    unsigned long long partitionTime{};
    std::vector<unsigned long long> queryTimes;
    std::vector<unsigned long long> addArcTimes;
    std::vector<unsigned long long> removeArcTimes;

    void addInitTime(TimePoint start, TimePoint end){
        initTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end -start).count();
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

    double getAvgQueryTime(){
        return getAvg(queryTimes);
    }

    double getAvgAddArcTime(){
        return getAvg(addArcTimes);
    }

    double getAvgRemoveArcTime(){
        return getAvg(removeArcTimes);
    }

    unsigned long long getQueryTime(){
        return std::accumulate(queryTimes.begin(), queryTimes.end(), 0ULL);
    }

    unsigned long long getAddArcTime(){
        return std::accumulate(addArcTimes.begin(), addArcTimes.end(), 0ULL);
    }

    unsigned long long getRemoveArcTime(){
        return std::accumulate(removeArcTimes.begin(), removeArcTimes.end(), 0ULL);
    }

    unsigned long long getAllTime(){
        return getQueryTime() + getAddArcTime() + getRemoveArcTime() + initTime;
    }

    void addPartitionTime(TimePoint start, TimePoint end) {
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        partitionTime = duration;
    }

private:
    static double getAvg(const std::vector<unsigned long long >& times){
        unsigned long long sum = 0ULL;
        for(unsigned long long time : times){
            sum += time;
        }
        double avg = sum;
        return avg/times.size();
    }
};


void
AlgorithmTester::runTest(DynamicAPReachAlgorithm *algorithm, TimeCollector* timer) {

    auto* queries = instanceProvider->getQueries();
    auto* dynGraph = instanceProvider->getGraph();
    auto* diGraph = dynGraph->getDiGraph();

    //measurement specific
    algorithm->setAutoUpdate(false);
    algorithm->setGraph(diGraph);

    std::cout << "Initializing next Algorithm " << std::endl;

    auto startTime = HRC::now();
    algorithm->run();
    auto endTime = HRC::now();
    timer->addInitTime(startTime, endTime);
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

    double advantage = 0.025;

    bool first = true;
    for (const auto &currentQueries : *queries) {

        if(first) first=false;
        else for (auto j = 0ULL; currentQueries.size() != 0ULL && j < currentQueries.size() - 1; j += 2) {

            auto startVertex = dynGraph->getCurrentVertexForId(currentQueries[j]);
            auto endVertex = dynGraph->getCurrentVertexForId(currentQueries[j + 1]);


            auto startQueryTime = std::chrono::high_resolution_clock::now();
            algorithm->query(startVertex, endVertex);
            auto endQueryTime = std::chrono::high_resolution_clock::now();
            timer->addQueryTime(startQueryTime, endQueryTime);
        }

        if( ++currentStep > nextReport){
            nextReport += reportStep;

            double progPercentage = double (currentStep) / queries->size();

            auto currentProg = short(progPercentage * 100.0);
            if(progress < currentProg){
                progress = currentProg;
                std::cout << progress << "% at " << timer->getAllTime() << std::endl;
            }

            if(timeOut && ( double(timeOut) * (progPercentage + advantage) < double(timer->getAllTime()))){
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





    std::cout << "Finished\n";

}

void AlgorithmTester::runTests() {

    writeHeader();

    for(DynamicAPReachAlgorithm* algorithm: algorithms){
        resetDynGraph();

        TimeCollector timer{};
        runTest(algorithm, &timer);

        writeResults(&timer);
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
    file << "," << timer->getAllTime();
    file << "," << timer->timedOut;
    file << "," << "\"" << timer->error << "\"";

    file << std::endl;
}
