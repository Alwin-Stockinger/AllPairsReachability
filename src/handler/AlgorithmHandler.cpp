//
// Created by Alwin Stockinger.
//

#include <iostream>
#include <chrono>
#include "AlgorithmHandler.h"
#include "../converter/GraphFileConverter.h"
#include <set>
#include <fstream>
#include <numeric>
#include <property/fastpropertymap.h>
#include <spawn.h>
#include <wait.h>

typedef std::chrono::high_resolution_clock HRC;
typedef std::chrono::high_resolution_clock::time_point TimePoint;

struct AlgorithmHandler::TimeCollector {


    explicit TimeCollector(DynamicAPReachAlgorithm *algorithm, const unsigned long long int k) {
        this->algorithm = algorithm;
        this->k = k;
    }

    DynamicAPReachAlgorithm *algorithm = nullptr;
    unsigned long long k;

    std::vector<unsigned long long> queryTimes;
    std::vector<unsigned long long> addArcTimes;
    std::vector<unsigned long long> removeArcTimes;

    void addQueryTime(TimePoint start, TimePoint end) {
        unsigned long long duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        queryTimes.push_back(duration);
    }

    void addAddArcTime(TimePoint start, TimePoint end) {
        unsigned long long duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        addArcTimes.push_back(duration);
    }

    void addRemoveArcTime(TimePoint start, TimePoint end) {
        unsigned long long duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        removeArcTimes.push_back(duration);
    }

    const double getAvgQueryTime(){
        return getAvg(queryTimes);
    }

    const double getAvgAddArcTime(){
        return getAvg(addArcTimes);
    }

    const double getAvgRemoveArcTime(){
        return getAvg(removeArcTimes);
    }

    const unsigned long long getQueryTime(){
        return std::accumulate(queryTimes.begin(), queryTimes.end(), 0ULL);
    }

    const unsigned long long getAddArcTime(){
        return std::accumulate(addArcTimes.begin(), addArcTimes.end(), 0ULL);
    }

    const unsigned long long getRemoveArcTime(){
        return std::accumulate(removeArcTimes.begin(), removeArcTimes.end(), 0ULL);
    }

    const unsigned long long getAllTime(){
        return getQueryTime() + getAddArcTime() + getRemoveArcTime();
    }

private:
    static const double getAvg(const std::vector<unsigned long long >& times){
        unsigned long long sum = 0ULL;
        for(unsigned long long time : times){
            sum += time;
        }
        double avg = sum;
        return avg/times.size();
    }
};


enum class MenuOptions{ reach=1, addArc=2, removeArc=3, quit=0} option;

void AlgorithmHandler::runInterface() {
    std::cout << "All Pair Reachability Suite\n";

    option = MenuOptions ::reach;

    while(option != MenuOptions::quit){
        std::cout
                << std::endl
                << "1. Check reachable\n"
                << "2. Add arc\n"
                << "3. Remove arc\n"
                <<"0. Exit\n";

        int intOption = 0;
        std::cin >> intOption;
        option = static_cast<MenuOptions>(intOption);

        switch (option) {
            case MenuOptions ::reach:
                reachabilityCheck();
                break;
            case MenuOptions ::addArc:
                addArc();
                break;
            case MenuOptions ::removeArc:
                removeArc();
                break;
            case MenuOptions::quit:
                break;
        }
    }
}

void AlgorithmHandler::reachabilityCheck() {

    unsigned long long startId = 0, endId = 0;

    std::cout << "Start Vertex: ";
    std::cin >> startId;

    std::cout << "End Vertex: ";
    std::cin >> endId;

    Algora::DynamicDiGraph& graph = instanceProvider->getGraph();

    for( DynamicAPReachAlgorithm* algorithm : algorithms){

        std::cout << algorithm->getName() << ": ";

        if(algorithm->query(graph.getCurrentVertexForId(startId), graph.getCurrentVertexForId(endId))){
            std::cout << "True\n";
        }
        else{
            std::cout << "False\n";
        }
    }
}

void AlgorithmHandler::addArc() {

    unsigned long long startId = 0, endId = 0;

    std::cout << "Start Vertex: ";
    std::cin >> startId;

    std::cout << "End Vertex: ";
    std::cin >> endId;

    Algora::DynamicDiGraph& graph = instanceProvider->getGraph();

    graph.addArc(startId, endId, graph.getCurrentTime()+1);
    graph.applyNextDelta();
}

void AlgorithmHandler::removeArc(){

    unsigned long long startId = 0, endId = 0;

    std::cout << "Start Vertex: ";
    std::cin >> startId;

    std::cout << "End Vertex: ";
    std::cin >> endId;

    Algora::DynamicDiGraph& graph = instanceProvider->getGraph();

    graph.removeArc(startId, endId, graph.getCurrentTime()+1);
    graph.applyNextDelta();
}

void AlgorithmHandler::runTests(unsigned long long const k) {

    std::vector<TimeCollector*> timers;

    auto &queries = instanceProvider->getQueries();
    auto &dynGraph = instanceProvider->getGraph();
    auto* diGraph = dynGraph.getDiGraph();

    for(unsigned long long i = 2; i <= k ; i++){

        for(auto* algorithm:algorithms) {

            algorithm->setAutoUpdate(false);
            algorithm->setGraph(diGraph);
            algorithm->setPartitionFunction(GraphFileConverter::handlePartitioning, k);
            algorithm->partition();

            std::cout << "Starting Algorithm " << algorithm->getBaseName() << std::endl;

            auto* timer = new TimeCollector(algorithm, i);
            timers.push_back(timer);

            auto onArcAdded = [algorithm, &timer](Algora::Arc* arc){
                auto startTime = HRC::now();
                algorithm->onArcAdd(arc);
                auto endTime = HRC::now();
                timer->addAddArcTime(startTime, endTime);
            };
            diGraph->onArcAdd(algorithm,onArcAdded);

            auto onArcRemoved = [algorithm, &timer](Algora::Arc* arc){
                auto startTime = HRC::now();
                algorithm->onArcRemove(arc);
                auto endTime = HRC::now();
                timer->addRemoveArcTime(startTime, endTime);
            };
            diGraph->onArcRemove(algorithm, onArcRemoved);

            auto onVertexAdded = [algorithm, &timer](Algora::Vertex* vertex){
                algorithm->onVertexAdd(vertex);
            };
            diGraph->onVertexAdd(algorithm, onVertexAdded);

            auto onVertexRemoved = [algorithm, &timer](Algora::Vertex* vertex){
                algorithm->onVertexRemove(vertex);
            };
            diGraph->onVertexRemove(algorithm, onVertexRemoved);

            for (auto &currentQueries : queries) {
                for (auto j = 0ULL; currentQueries.size() != 0ULL && j < currentQueries.size() - 1; j += 2) {

                    auto startVertex = dynGraph.getCurrentVertexForId(currentQueries[j]);
                    auto endVertex = dynGraph.getCurrentVertexForId(currentQueries[j + 1]);


                    auto startTime = std::chrono::high_resolution_clock::now();
                    algorithm->query(startVertex, endVertex);
                    auto endTime = std::chrono::high_resolution_clock::now();
                    timer->addQueryTime(startTime, endTime);
                }
                dynGraph.applyNextDelta();
            }

            algorithm->unsetGraph();

            diGraph->removeOnArcAdd(algorithm);
            diGraph->removeOnArcRemove(algorithm);
            diGraph->removeOnVertexAdd(algorithm);
            diGraph->removeOnVertexRemove(algorithm);


            //reset to initial graph
            dynGraph.resetToBigBang();
            dynGraph.applyNextDelta();
        }
    }

    writeResults(timers);

    for(TimeCollector* timer : timers){
        delete timer;
    }
}

void AlgorithmHandler::writeResults (const std::vector<TimeCollector*>& timers){

    std::ofstream file;
    file.open("results.csv");

    file << instanceProvider->getConfiguration() << std::endl;

    file << "k";
    file << ",SS Base Algorithm";
    file << ",avg query time(ns)";
    file << ",avg add Arc time(ns)";
    file << ",avg remove Arc time(ns)";
    file << ",whole Time(ns)";

    file << std::endl;

    for(TimeCollector* timer: timers){
        file << timer->k;
        file << "," << timer->algorithm->getBaseName();
        file << "," << timer->getAvgQueryTime();
        file << "," << timer->getAvgAddArcTime();
        file << "," << timer->getAvgRemoveArcTime();
        file << "," << timer->getAllTime();
        file << std::endl;
    }
}
