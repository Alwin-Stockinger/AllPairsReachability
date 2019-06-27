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

#include <algorithm.reach/staticbfsssreachalgorithm.h>
#include <algorithm.reach/staticdfsssreachalgorithm.h>
#include <algorithm.reach/lazydfsssreachalgorithm.h>
#include <algorithm.reach/lazybfsssreachalgorithm.h>
#include <algorithm.reach/cachingdfsssreachalgorithm.h>
#include <algorithm.reach/cachingbfsssreachalgorithm.h>
#include <algorithm.reach/simpleincssreachalgorithm.h>
#include <algorithm.reach.es/estree-ml.h>
#include <algorithm.reach.es/estree-bqueue.h>
#include <algorithm.reach.es/estree-queue.h>
#include <algorithm.reach.es/simpleestree.h>

#include "../algorithm.reach.allPair/SSBasedDAPReachAlgorithmImplementation.h"
#include "../partition/Partitioner.h"

typedef std::chrono::high_resolution_clock HRC;
typedef std::chrono::high_resolution_clock::time_point TimePoint;

struct AlgorithmHandler::TimeCollector {


    explicit TimeCollector(const unsigned long long int k) : k(k) {}


    std::string algorithmName;
    std::string error ="";

    const unsigned long long k;

    unsigned long long partitionTime{};
    std::vector<unsigned long long> queryTimes;
    std::vector<unsigned long long> addArcTimes;
    std::vector<unsigned long long> removeArcTimes;

    void addPartitionTime(TimePoint start, TimePoint end){
        partitionTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end -start).count();
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
        return getQueryTime() + getAddArcTime() + getRemoveArcTime() + partitionTime;
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

    throw std::runtime_error("not implemented");
    /*for( DynamicAPReachAlgorithm* algorithm : algorithms){

        std::cout << algorithm->getName() << ": ";

        if(algorithm->query(graph.getCurrentVertexForId(startId), graph.getCurrentVertexForId(endId))){
            std::cout << "True\n";
        }
        else{
            std::cout << "False\n";
        }
    }*/
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

void AlgorithmHandler::runTests(unsigned long long const kMax, const std::vector<std::string>& algorithmNames) {

    auto &queries = instanceProvider->getQueries();
    auto &dynGraph = instanceProvider->getGraph();
    auto* diGraph = dynGraph.getDiGraph();

    writeHeader();

    for(unsigned long long i = 2; i <= kMax ; i++){

        auto* algorithms = createAlgorithms(algorithmNames, i);
        for(auto* algorithm: (*algorithms)) {

            TimeCollector timer(i);

            //graph set + partitioning
            algorithm->setAutoUpdate(false);
            auto startTime = HRC::now();
            algorithm->setGraph(diGraph);
            auto endTime = HRC::now();
            timer.addPartitionTime(startTime, endTime);

            timer.algorithmName = algorithm->getBaseName();

            std::cout << "Starting Algorithm " << algorithm->getBaseName() << std::endl;

            auto onArcAdded = [algorithm, &timer](Algora::Arc* arc){
                auto startTime = HRC::now();
                algorithm->onArcAdd(arc);
                auto endTime = HRC::now();
                timer.addAddArcTime(startTime, endTime);
            };
            diGraph->onArcAdd(algorithm,onArcAdded);

            auto onArcRemoved = [algorithm, &timer](Algora::Arc* arc){
                auto startTime = HRC::now();
                algorithm->onArcRemove(arc);
                auto endTime = HRC::now();
                timer.addRemoveArcTime(startTime, endTime);
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


                    auto startQueryTime = std::chrono::high_resolution_clock::now();
                    algorithm->query(startVertex, endVertex);
                    auto endQueryTime = std::chrono::high_resolution_clock::now();
                    timer.addQueryTime(startQueryTime, endQueryTime);
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

            std::cout << "Finished\n";

            delete algorithm;

            writeResults(timer);
        }
        delete algorithms;
    }
}

void AlgorithmHandler::writeHeader(){
    std::ofstream file;

    file.open("results.csv", std::ios_base::app);

    file << instanceProvider->getConfiguration() << std::endl;

    file << "k";
    file << ",SS Base Algorithm";
    file << ",avg query time(ns)";
    file << ",avg add Arc time(ns)";
    file << ",avg remove Arc time(ns)";
    file << ",partition time(ns)";
    file << ",whole Time(ns)";
    file << ",error";
    file << std::endl;

}

void AlgorithmHandler::writeResults(TimeCollector& timer) {
    std::ofstream file;
    file.open("results.csv", std::ios_base::app);

    file << timer.k;
    file << "," << "\"" << timer.algorithmName << "\"";
    file << "," << timer.getAvgQueryTime();
    file << "," << timer.getAvgAddArcTime();
    file << "," << timer.getAvgRemoveArcTime();
    file << "," << timer.partitionTime;
    file << "," << timer.getAllTime();
    file << "," << "\"" << timer.error << "\"";

    file << std::endl;
}

std::vector<DynamicAPReachAlgorithm *> * AlgorithmHandler::createAlgorithms(
        const std::vector<std::string> &algorithmNames, const unsigned long long k) {

    auto *algorithms = new std::vector<DynamicAPReachAlgorithm*>;

    for(const std::string& algorithmName: algorithmNames){

        if(algorithmName == "StaticBFS") {
            algorithms->push_back(
                    createAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::StaticBFSSSReachAlgorithm>>(k));
        }
        else if(algorithmName == "StaticDFS") {
            algorithms->push_back(
                    createAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::StaticDFSSSReachAlgorithm>>(k));
        }
        else if( algorithmName == "LazyDFS") {
            algorithms->push_back(
                    createAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::LazyDFSSSReachAlgorithm>>(k));
        }
        else if( algorithmName == "LazyBFS") {
            algorithms->push_back(
                    createAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::LazyBFSSSReachAlgorithm>>(k));
        }
        else if( algorithmName == "CachingDFS") {
            algorithms->push_back(
                    createAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::CachingDFSSSReachAlgorithm>>(k));
        }
        else if( algorithmName == "CachingBFS") {
            algorithms->push_back(
                    createAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::CachingBFSSSReachAlgorithm>>(k));
        }
        else if( algorithmName == "SimpleInc") {
            algorithms->push_back(
                    createAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::SimpleIncSSReachAlgorithm>>(k));
        }
        else if( algorithmName == "ESTreeML") {
            algorithms->push_back(
                    createAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::ESTreeML>>(k));
        }
        else if( algorithmName == "OldESTree") {
            algorithms->push_back(
                    createAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::OldESTree>>(k));
        }
        else if( algorithmName == "ESTreeQ") {
            algorithms->push_back(
                    createAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::ESTreeQ>>(k));
        }
        else if( algorithmName == "SimpleESTree") {
            algorithms->push_back(
                    createAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::SimpleESTree>>(k));
        }
        else{
            std::cerr << algorithmName << " not a viable algorithm" << std::endl;
            //TODO throw error
        }
    }
    return algorithms;
}

template<typename T, unsigned Level>
DynamicAPReachAlgorithm  *AlgorithmHandler::createAlgorithm(unsigned long long k) {
    auto* algorithm = new PartitionedDAPReachAlgorithmImplementation<T,Level>();
    algorithm->setK(k);
    return algorithm;
}