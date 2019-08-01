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
#include "../algorithm.reach.allPair/SuperVertexPDAPReachAlgorithm.h"
#include "../algorithm.reach.allPair/SuperVertexAlgorithmImplementation.h"

typedef std::chrono::high_resolution_clock HRC;
typedef HRC::time_point TimePoint;

struct AlgorithmHandler::TimeCollector {


    explicit TimeCollector(const unsigned long long int k, unsigned int depth) : k(k), depth(depth)  {}


    std::string algorithmName;
    std::string error ="";
    bool timedOut = false;

    const unsigned long long k;
    const unsigned depth;

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
                << "0. Exit\n";

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

    Algora::DynamicDiGraph* graph = instanceProvider->getGraph();

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

    Algora::DynamicDiGraph* graph = instanceProvider->getGraph();

    graph->addArc(startId, endId, graph->getCurrentTime()+1);
    graph->applyNextDelta();
}

void AlgorithmHandler::removeArc(){

    unsigned long long startId = 0, endId = 0;

    std::cout << "Start Vertex: ";
    std::cin >> startId;

    std::cout << "End Vertex: ";
    std::cin >> endId;

    Algora::DynamicDiGraph* graph = instanceProvider->getGraph();

    graph->removeArc(startId, endId, graph->getCurrentTime()+1);
    graph->applyNextDelta();
}

void
AlgorithmHandler::runTest(DynamicAPReachAlgorithm *algorithm, TimeCollector &timer, const unsigned long long &timeOut) {

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
    timer.addInitTime(startTime, endTime);
    std::cout << "Initialized in " << timer.initTime << "ns" << std::endl;
    std::cout << "Starting Algorithm " << algorithm->getBaseName() << std::endl;

    timer.algorithmName = algorithm->getBaseName();



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

    short progress = 0;
    unsigned long long currentStep = 0ULL;
    unsigned long long nextReport = 0ULL;
    unsigned long long reportStep = 1ULL;

    for (const auto &currentQueries : *queries) {

        auto queriesStart = std::chrono::high_resolution_clock::now();
        for (auto j = 0ULL; currentQueries.size() != 0ULL && j < currentQueries.size() - 1; j += 2) {

            auto startVertex = dynGraph->getCurrentVertexForId(currentQueries[j]);
            auto endVertex = dynGraph->getCurrentVertexForId(currentQueries[j + 1]);


            auto startQueryTime = std::chrono::high_resolution_clock::now();
            algorithm->query(startVertex, endVertex);
            auto endQueryTime = std::chrono::high_resolution_clock::now();
            timer.addQueryTime(startQueryTime, endQueryTime);
        }
        auto queriesEnd = std::chrono::high_resolution_clock::now();

        std::cout   << "Queries: "
                    << std::chrono::duration_cast<std::chrono::nanoseconds>(queriesEnd - queriesStart).count()
                    << std::endl;


        if( ++currentStep > nextReport){
            nextReport += reportStep;

            int currentProg = (currentStep*100ULL) / queries->size();
            if(progress < currentProg){
                progress = currentProg;
                std::cout << progress << "% at " << timer.getAllTime() << std::endl;
            }

            if(timeOut && timeOut < timer.getAllTime()){
                std::cout << "TIMEOUT" << std::endl;
                timer.timedOut = true;
                break;
            }
        }

        auto deltaStart = std::chrono::high_resolution_clock::now();
        dynGraph->applyNextDelta();
        auto deltaEnd = std::chrono::high_resolution_clock::now();

        std::cout   << "Delta: "
                    << std::chrono::duration_cast<std::chrono::nanoseconds>(deltaEnd - deltaStart).count()
                    << std::endl;
    }

    algorithm->unsetGraph();

    diGraph->removeOnArcAdd(algorithm);
    diGraph->removeOnArcRemove(algorithm);
    diGraph->removeOnVertexAdd(algorithm);
    diGraph->removeOnVertexRemove(algorithm);



    //reset to initial graph
    dynGraph->resetToBigBang();
    dynGraph->applyNextDelta();

    //Fix for very strange bug that doesn't always happen
    dynGraph->resetToBigBang();
    dynGraph->applyNextDelta();

    std::cout << "Finished\n";

}

void
AlgorithmHandler::runTests(const std::vector<std::string> &algorithmNames, const bool exponentialK = false,
                           const unsigned long long int kMax = 2ULL, const unsigned long long int kMin = 2ULL,
                           const unsigned long long int timeOut = 0ULL, const bool detailedResults = false,
                           const unsigned int minLevel = 0U, const unsigned int maxLevel = 0U,
                           const bool withoutPartition = false,
                           const std::vector<std::string> *const overlayNames = nullptr,
                           const unsigned long long repartitionThreshold = 0ULL, bool normalAlgorithmTests = true,
                           bool superVertexAlgorithmTests = false) {




    writeHeader();

    if(detailedResults){
        writeDetailedHeader();
    }

    Algora::DiGraph* diGraph = instanceProvider->getGraph()->getDiGraph();

    //SSBased Algorithm tests
    if(withoutPartition){
        auto* algorithms = createAPAlgorithms(algorithmNames);

        for(auto* algorithm : (*algorithms)) {

            TimeCollector timer( 1U, 0U);

            runTest(algorithm, timer, timeOut);

            delete algorithm;

            writeResults(timer);

            if(detailedResults){
                writeDetailedResults(timer);
            }

        }
        delete algorithms;
    }


    if(normalAlgorithmTests) {
        for (unsigned long long k = kMin; k <= kMax; [exponentialK, &k]() {
            exponentialK ? (k *= 2ULL) : (k++);
        }()) {

            //auto parStartTime = HRC::now();
            /*Algora::FastPropertyMap<unsigned long long> partition
                = Partitioner::handlePartitioning(k, diGraph);*/
            //auto parEndTime = HRC::now();

            for (unsigned depth = minLevel; depth <= maxLevel; depth++) {

                std::vector<DynamicAPReachAlgorithm *> *algorithms;


                if (overlayNames) {
                    algorithms = createSpecialOverlayPartitionedAlgorithms(algorithmNames, *overlayNames, k, NULL,
                                                                           depth, repartitionThreshold);
                } else {
                    algorithms = createPartitionedAlgorithms(algorithmNames, k, NULL, depth, repartitionThreshold);
                }

                for (auto *algorithm: (*algorithms)) {

                    TimeCollector timer(k, depth);

                    runTest(algorithm, timer, timeOut);

                    delete algorithm;


                    writeResults(timer);

                    if (detailedResults) {
                        writeDetailedResults(timer);
                    }

                }

                delete algorithms;
            }
        }
    }

    if(superVertexAlgorithmTests){
        for (unsigned long long k = kMin; k <= kMax; [exponentialK, &k]() {
            exponentialK ? (k *= 2ULL) : (k++);
        }()) {

            //auto parStartTime = HRC::now();
            /*Algora::FastPropertyMap<unsigned long long> partition
                = Partitioner::handlePartitioning(k, diGraph);*/
            //auto parEndTime = HRC::now();

            for (unsigned depth = minLevel; depth <= maxLevel; depth++) {

                std::vector<DynamicAPReachAlgorithm *> *algorithms;


                if (overlayNames) {
                    algorithms = createSpecialOverlaySuperVertexAlgorithms(algorithmNames, *overlayNames, k,
                                                                           depth, repartitionThreshold);
                } else {
                    algorithms = createSuperVertexAlgorithms(algorithmNames, k, depth, repartitionThreshold);
                }

                for (auto *algorithm: (*algorithms)) {

                    TimeCollector timer(k, depth);

                    runTest(algorithm, timer, timeOut);

                    delete algorithm;


                    writeResults(timer);

                    if (detailedResults) {
                        writeDetailedResults(timer);
                    }

                }

                delete algorithms;
            }
        }
    }

}

void AlgorithmHandler::writeHeader(){
    std::ofstream file;

    file.open("results.csv", std::ios_base::app);

    file << instanceProvider->getConfiguration() << std::endl;

    file << "k";
    file << ",depth";
    file << ",SS Base Algorithm";
    file << ",avg query time(ns)";
    file << ",avg add Arc time(ns)";
    file << ",avg remove Arc time(ns)";
    file << ",init time(ns)";
    file << ",whole Time(ns)";
    file << ",partition time(ns)";
    file << ",timeout";
    file << ",error";
    file << std::endl;

}

void AlgorithmHandler::writeResults(TimeCollector& timer) {
    std::ofstream file;
    file.open("results.csv", std::ios_base::app);

    file << timer.k;
    file << "," << timer.depth;
    file << "," << "\"" << timer.algorithmName << "\"";
    file << "," << timer.getAvgQueryTime();
    file << "," << timer.getAvgAddArcTime();
    file << "," << timer.getAvgRemoveArcTime();
    file << "," << timer.initTime;
    file << "," << timer.getAllTime();
    file << "," << timer.partitionTime;
    file << "," << timer.timedOut;
    file << "," << "\"" << timer.error << "\"";

    file << std::endl;
}

template<typename OverlayAlgorithm>
std::vector<DynamicAPReachAlgorithm *>
AlgorithmHandler::createPartitionedAlgorithmForOverlay(const std::vector<std::string> &algorithmNames,
                                                       const unsigned long long int k,
                                                       const Algora::FastPropertyMap<unsigned long long int> &partitions,
                                                       const unsigned depth,
                                                       const unsigned long long repartitionThreshold) {

    std::vector<DynamicAPReachAlgorithm*> algorithms;

    /*std::function<Algora::FastPropertyMap<unsigned long long>(unsigned long long int, Algora::DiGraph*)>
            partitionFunction = [partitions] (unsigned long long k, Algora::DiGraph* diGraph){

        return partitions;
    };*/


    for(const std::string& algorithmName: algorithmNames){

        PartitionedDAPReachAlgorithm *algorithm = nullptr;

        if(algorithmName == "StaticBFS") {
            algorithm =
                    createPartitionAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::StaticBFSSSReachAlgorithm>, SSBasedDAPReachAlgorithmImplementation<OverlayAlgorithm>>(depth);
        }
        else if(algorithmName == "StaticDFS") {
            algorithm =
                    createPartitionAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::StaticDFSSSReachAlgorithm>, SSBasedDAPReachAlgorithmImplementation<OverlayAlgorithm>>(depth);
        }
        else if( algorithmName == "LazyDFS") {
            algorithm =
                    createPartitionAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::LazyDFSSSReachAlgorithm,false>, SSBasedDAPReachAlgorithmImplementation<OverlayAlgorithm,true>>(depth);
        }
        else if( algorithmName == "LazyBFS") {
            algorithm =
                    createPartitionAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::LazyBFSSSReachAlgorithm,false>, SSBasedDAPReachAlgorithmImplementation<OverlayAlgorithm, true>>(depth);
        }
        else if( algorithmName == "CachingDFS") {
            algorithm =
                    createPartitionAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::CachingDFSSSReachAlgorithm,false>, SSBasedDAPReachAlgorithmImplementation<OverlayAlgorithm, true>>(depth);
        }
        else if( algorithmName == "CachingBFS") {
            algorithm =
                    createPartitionAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::CachingBFSSSReachAlgorithm,false>, SSBasedDAPReachAlgorithmImplementation<OverlayAlgorithm, true>>(depth);
        }
        else if( algorithmName == "SimpleInc") {
            algorithm =
                    createPartitionAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::SimpleIncSSReachAlgorithm,false>, SSBasedDAPReachAlgorithmImplementation<OverlayAlgorithm, true>>(depth);
        }
        else if( algorithmName == "ESTreeML") {
            algorithm =
                    createPartitionAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::ESTreeML,false>, SSBasedDAPReachAlgorithmImplementation<OverlayAlgorithm, true>>(depth);
        }
        else if( algorithmName == "OldESTree") {
            algorithm =
                    createPartitionAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::OldESTree,false>, SSBasedDAPReachAlgorithmImplementation<OverlayAlgorithm, true>>(depth);
        }
        else if( algorithmName == "ESTreeQ") {
            algorithm =
                    createPartitionAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::ESTreeQ,false>, SSBasedDAPReachAlgorithmImplementation<OverlayAlgorithm, true>>(depth);
        }
        else if( algorithmName == "SimpleESTree") {
            algorithm =
                    createPartitionAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::SimpleESTree,false>, SSBasedDAPReachAlgorithmImplementation<OverlayAlgorithm, true>>(depth);
        }
        else{
            std::cerr << algorithmName << " not a viable algorithm" << std::endl;
            continue;
            //TODO throw error
        }
        //algorithm->setPartitionFunction(partitionFunction, k);
        algorithm->setK(k);
        algorithm->setRepartitionThreshold(repartitionThreshold);
        algorithms.push_back(algorithm);
    }
    return algorithms;
}

std::vector<DynamicAPReachAlgorithm *> * AlgorithmHandler::createSpecialOverlayPartitionedAlgorithms(
        const std::vector<std::string> &algorithmNames, const std::vector<std::string> &overlayNames,
        unsigned long long int k,
        const Algora::FastPropertyMap<unsigned long long int>& partitions,
        unsigned depth = 0U, const unsigned long long repartitionThreshold = 0ULL){

    auto *algorithms = new std::vector<DynamicAPReachAlgorithm*>;

    for(const std::string& overlayName: overlayNames){

        std::vector<DynamicAPReachAlgorithm*> overlayAlgorithms;

        if(overlayName == "StaticBFS") {
            overlayAlgorithms =
                    createPartitionedAlgorithmForOverlay<Algora::StaticBFSSSReachAlgorithm>(algorithmNames, k,
                                                                                            partitions, depth, repartitionThreshold);
        }
        else if(overlayName == "StaticDFS") {
            overlayAlgorithms =
                    createPartitionedAlgorithmForOverlay<Algora::StaticDFSSSReachAlgorithm>(algorithmNames, k,
                                                                                            partitions, depth, repartitionThreshold);
        }
        else if( overlayName == "LazyDFS") {
            overlayAlgorithms =
                    createPartitionedAlgorithmForOverlay<Algora::LazyDFSSSReachAlgorithm>(algorithmNames, k, partitions,
                                                                                          depth, repartitionThreshold);
        }
        else if( overlayName == "LazyBFS") {
            overlayAlgorithms =
                    createPartitionedAlgorithmForOverlay<Algora::LazyBFSSSReachAlgorithm>(algorithmNames, k, partitions,
                                                                                          depth, repartitionThreshold);
        }
        else if( overlayName == "CachingDFS") {
            overlayAlgorithms =
                    createPartitionedAlgorithmForOverlay<Algora::CachingDFSSSReachAlgorithm>(algorithmNames, k,
                                                                                             partitions, depth, repartitionThreshold);
        }
        else if( overlayName == "CachingBFS") {
            overlayAlgorithms =
                    createPartitionedAlgorithmForOverlay<Algora::CachingBFSSSReachAlgorithm>(algorithmNames, k,
                                                                                             partitions, depth, repartitionThreshold);
        }
        else if( overlayName == "SimpleInc") {
            overlayAlgorithms =
                    createPartitionedAlgorithmForOverlay<Algora::SimpleIncSSReachAlgorithm>(algorithmNames, k,
                                                                                            partitions, depth, repartitionThreshold);
        }
        else if( overlayName == "ESTreeML") {
            overlayAlgorithms =
                    createPartitionedAlgorithmForOverlay<Algora::ESTreeML>(algorithmNames, k, partitions, depth, repartitionThreshold);
        }
        else if( overlayName == "OldESTree") {
            overlayAlgorithms =
                    createPartitionedAlgorithmForOverlay<Algora::OldESTree>(algorithmNames, k, partitions, depth, repartitionThreshold);
        }
        else if( overlayName == "ESTreeQ") {
            overlayAlgorithms =
                    createPartitionedAlgorithmForOverlay<Algora::ESTreeQ>(algorithmNames, k, partitions, depth, repartitionThreshold);
        }
        else if( overlayName == "SimpleESTree") {
            overlayAlgorithms =
                    createPartitionedAlgorithmForOverlay<Algora::SimpleESTree>(algorithmNames, k, partitions, depth, repartitionThreshold);
        }
        else{
            std::cerr << overlayName << " not a viable algorithm" << std::endl;
            //TODO throw error
        }

        algorithms->insert(algorithms->end(), overlayAlgorithms.begin(), overlayAlgorithms.end());
    }
    return algorithms;

}


std::vector<DynamicAPReachAlgorithm *> * AlgorithmHandler::createPartitionedAlgorithms(
        const std::vector<std::string> &algorithmNames,
        const unsigned long long int k,
        const Algora::FastPropertyMap<unsigned long long int> &partitions,
        const unsigned int depth = 0U,
        const unsigned long long repartitionThreshold) {

    auto *algorithms = new std::vector<DynamicAPReachAlgorithm*>;

    /*std::function<Algora::FastPropertyMap<unsigned long long>(unsigned long long int, Algora::DiGraph*)>
            partitionFunction = [partitions] (unsigned long long k, Algora::DiGraph* diGraph){

                            return partitions;
            };*/


    for(const std::string& algorithmName: algorithmNames){

        PartitionedDAPReachAlgorithm *algorithm = nullptr;

        if(algorithmName == "StaticBFS") {
            algorithm =
                    createPartitionAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::StaticBFSSSReachAlgorithm>>(depth);
        }
        else if(algorithmName == "StaticDFS") {
            algorithm =
                    createPartitionAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::StaticDFSSSReachAlgorithm>>(depth);
        }
        else if( algorithmName == "LazyDFS") {
            algorithm =
                    createPartitionAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::LazyDFSSSReachAlgorithm,true>>(depth);
        }
        else if( algorithmName == "LazyBFS") {
            algorithm =
                    createPartitionAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::LazyBFSSSReachAlgorithm,true>>(depth);
        }
        else if( algorithmName == "CachingDFS") {
            algorithm =
                    createPartitionAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::CachingDFSSSReachAlgorithm,true>>(depth);
        }
        else if( algorithmName == "CachingBFS") {
            algorithm =
                    createPartitionAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::CachingBFSSSReachAlgorithm,true>>(depth);
        }
        else if( algorithmName == "SimpleInc") {
            algorithm =
                    createPartitionAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::SimpleIncSSReachAlgorithm,true>>(depth);
        }
        else if( algorithmName == "ESTreeML") {
            algorithm =
                    createPartitionAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::ESTreeML,true>>(depth);
        }
        else if( algorithmName == "OldESTree") {
            algorithm =
                    createPartitionAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::OldESTree,true>>(depth);
        }
        else if( algorithmName == "ESTreeQ") {
            algorithm =
                    createPartitionAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::ESTreeQ,true>>(depth);
        }
        else if( algorithmName == "SimpleESTree") {
            algorithm =
                    createPartitionAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::SimpleESTree,true>>(depth);
        }
        else{
            std::cerr << algorithmName << " not a viable algorithm" << std::endl;
            //TODO throw error
        }
        //algorithm->setPartitionFunction(partitionFunction, k);
        algorithm->setK(k);
        algorithm->setRepartitionThreshold(repartitionThreshold);
        algorithms->push_back(algorithm);
    }
    return algorithms;
}

template<typename SubAlgorithm, typename OverlayAlgorithm>
PartitionedDAPReachAlgorithm * AlgorithmHandler::createPartitionAlgorithm(const unsigned depth) {
    return new SimplePartitionedDAPReachAlgorithmImplementation<SubAlgorithm, OverlayAlgorithm>(depth);
}

void AlgorithmHandler::writeDetailedResults(const AlgorithmHandler::TimeCollector& collector) {
    std::ofstream file;
    file.open("detailedResults.csv", std::ios_base::app);
    file << "k = " << collector.k << "\n";
    file << "depth = " << collector.depth << "\n";
    file << "algorithm = " << collector.algorithmName << "\n";

    file << std::endl;

    file << "Query times" << "\n";
    for( unsigned long long time : collector.queryTimes){
        file << time << "\n";
    }
    file << "\n";

    file << "Insertion times " << "\n";
    for(unsigned long long time : collector.addArcTimes){
        file << time << "\n";
    }
    file << "\n";

    file << "Deletion times" << "\n";
    for(unsigned long long time : collector.removeArcTimes){
        file << time << "\n";
    }
    file << "\n";

    file << std::endl; //flush and double empty lines to tell algorithms apart
}

void AlgorithmHandler::writeDetailedHeader() {
    std::ofstream file;

    file.open("detailedResults.csv", std::ios_base::app);

    file << instanceProvider->getConfiguration() << std::endl << std::endl;
}

std::vector<DynamicAPReachAlgorithm*>* AlgorithmHandler::createAPAlgorithms(const std::vector<std::string> &algorithmNames) {

    auto *algorithms = new std::vector<DynamicAPReachAlgorithm*>;

    for(const std::string& algorithmName: algorithmNames){

        DynamicAPReachAlgorithm *algorithm = nullptr;

        if(algorithmName == "StaticBFS") {
            algorithm =
                    new SSBasedDAPReachAlgorithmImplementation<Algora::StaticBFSSSReachAlgorithm>();
        }
        else if(algorithmName == "StaticDFS") {
            algorithm =
                    new SSBasedDAPReachAlgorithmImplementation<Algora::StaticDFSSSReachAlgorithm>();
        }
        else if( algorithmName == "LazyDFS") {
            algorithm =
                    new SSBasedDAPReachAlgorithmImplementation<Algora::LazyDFSSSReachAlgorithm,true>();
        }
        else if( algorithmName == "LazyBFS") {
            algorithm =
                    new SSBasedDAPReachAlgorithmImplementation<Algora::LazyBFSSSReachAlgorithm,true>();
        }
        else if( algorithmName == "CachingDFS") {
            algorithm =
                    new SSBasedDAPReachAlgorithmImplementation<Algora::CachingDFSSSReachAlgorithm,true>();
        }
        else if( algorithmName == "CachingBFS") {
            algorithm =
                    new SSBasedDAPReachAlgorithmImplementation<Algora::CachingBFSSSReachAlgorithm,true>();
        }
        else if( algorithmName == "SimpleInc") {
            algorithm =
                    new SSBasedDAPReachAlgorithmImplementation<Algora::SimpleIncSSReachAlgorithm,true>();
        }
        else if( algorithmName == "ESTreeML") {
            algorithm =
                    new SSBasedDAPReachAlgorithmImplementation<Algora::ESTreeML,true>();
        }
        else if( algorithmName == "OldESTree") {
            algorithm =
                    new SSBasedDAPReachAlgorithmImplementation<Algora::OldESTree,true>();
        }
        else if( algorithmName == "ESTreeQ") {
            algorithm =
                    new SSBasedDAPReachAlgorithmImplementation<Algora::ESTreeQ,true>();
        }
        else if( algorithmName == "SimpleESTree") {
            algorithm =
                    new SSBasedDAPReachAlgorithmImplementation<Algora::SimpleESTree,true>();
        }
        else{
            std::cerr << algorithmName << " not a viable algorithm" << std::endl;
            //TODO throw error
        }

        algorithms->push_back(algorithm);
    }
    return algorithms;
}

std::vector<DynamicAPReachAlgorithm *> *
AlgorithmHandler::createSpecialOverlaySuperVertexAlgorithms(const std::vector<std::string> &algorithmNames,
                                                            const std::vector<std::string> overlayNames,
                                                            unsigned long long int k, unsigned int depth,
                                                            const unsigned long long int repartitionThreshold) {
    auto *algorithms = new std::vector<DynamicAPReachAlgorithm*>;

    for(const std::string& overlayName: overlayNames){

        std::vector<DynamicAPReachAlgorithm*> overlayAlgorithms;

        if(overlayName == "StaticBFS") {
            overlayAlgorithms =
                    createSuperVertexAlgorithmForOverlay<Algora::StaticBFSSSReachAlgorithm>(algorithmNames, k,
                                                                                            depth, repartitionThreshold);
        }
        else if(overlayName == "StaticDFS") {
            overlayAlgorithms =
                    createSuperVertexAlgorithmForOverlay<Algora::StaticDFSSSReachAlgorithm>(algorithmNames, k,
                                                                                             depth, repartitionThreshold);
        }
        else if( overlayName == "LazyDFS") {
            overlayAlgorithms =
                    createSuperVertexAlgorithmForOverlay<Algora::LazyDFSSSReachAlgorithm>(algorithmNames, k,
                                                                                          depth, repartitionThreshold);
        }
        else if( overlayName == "LazyBFS") {
            overlayAlgorithms =
                    createSuperVertexAlgorithmForOverlay<Algora::LazyBFSSSReachAlgorithm>(algorithmNames, k,
                                                                                          depth, repartitionThreshold);
        }
        else if( overlayName == "CachingDFS") {
            overlayAlgorithms =
                    createSuperVertexAlgorithmForOverlay<Algora::CachingDFSSSReachAlgorithm>(algorithmNames, k,
                                                                                              depth, repartitionThreshold);
        }
        else if( overlayName == "CachingBFS") {
            overlayAlgorithms =
                    createSuperVertexAlgorithmForOverlay<Algora::CachingBFSSSReachAlgorithm>(algorithmNames, k,
                                                                                              depth, repartitionThreshold);
        }
        else if( overlayName == "SimpleInc") {
            overlayAlgorithms =
                    createSuperVertexAlgorithmForOverlay<Algora::SimpleIncSSReachAlgorithm>(algorithmNames, k,
                                                                                             depth, repartitionThreshold);
        }
        else if( overlayName == "ESTreeML") {
            overlayAlgorithms =
                    createSuperVertexAlgorithmForOverlay<Algora::ESTreeML>(algorithmNames, k,  depth, repartitionThreshold);
        }
        else if( overlayName == "OldESTree") {
            overlayAlgorithms =
                    createSuperVertexAlgorithmForOverlay<Algora::OldESTree>(algorithmNames, k,  depth, repartitionThreshold);
        }
        else if( overlayName == "ESTreeQ") {
            overlayAlgorithms =
                    createSuperVertexAlgorithmForOverlay<Algora::ESTreeQ>(algorithmNames, k,  depth, repartitionThreshold);
        }
        else if( overlayName == "SimpleESTree") {
            overlayAlgorithms =
                    createSuperVertexAlgorithmForOverlay<Algora::SimpleESTree>(algorithmNames, k,  depth, repartitionThreshold);
        }
        else{
            std::cerr << overlayName << " not a viable algorithm" << std::endl;
            //TODO throw error
        }

        algorithms->insert(algorithms->end(), overlayAlgorithms.begin(), overlayAlgorithms.end());
    }
    return algorithms;
}

template<typename OverlayAlgorithm>
std::vector<DynamicAPReachAlgorithm *>
AlgorithmHandler::createSuperVertexAlgorithmForOverlay(const std::vector<std::string> &algorithmNames,
                                                       const unsigned long long int k,
                                                       const unsigned depth,
                                                       const unsigned long long repartitionThreshold) {

    std::vector<DynamicAPReachAlgorithm*> algorithms;

    /*std::function<Algora::FastPropertyMap<unsigned long long>(unsigned long long int, Algora::DiGraph*)>
            partitionFunction = [partitions] (unsigned long long k, Algora::DiGraph* diGraph){

        return partitions;
    };*/


    for(const std::string& algorithmName: algorithmNames){

        PartitionedDAPReachAlgorithm *algorithm = nullptr;

        if(algorithmName == "StaticBFS") {
            algorithm =
                    createSuperVertexAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::StaticBFSSSReachAlgorithm>, OverlayAlgorithm>(depth);
        }
        else if(algorithmName == "StaticDFS") {
            algorithm =
                    createSuperVertexAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::StaticDFSSSReachAlgorithm>, OverlayAlgorithm>(depth);
        }
        else if( algorithmName == "LazyDFS") {
            algorithm =
                    createSuperVertexAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::LazyDFSSSReachAlgorithm,false>, OverlayAlgorithm>(depth);
        }
        else if( algorithmName == "LazyBFS") {
            algorithm =
                    createSuperVertexAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::LazyBFSSSReachAlgorithm,false>, OverlayAlgorithm>(depth);
        }
        else if( algorithmName == "CachingDFS") {
            algorithm =
                    createSuperVertexAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::CachingDFSSSReachAlgorithm,false>, OverlayAlgorithm>(depth);
        }
        else if( algorithmName == "CachingBFS") {
            algorithm =
                    createSuperVertexAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::CachingBFSSSReachAlgorithm,false>, OverlayAlgorithm>(depth);
        }
        else if( algorithmName == "SimpleInc") {
            algorithm =
                    createSuperVertexAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::SimpleIncSSReachAlgorithm,false>, OverlayAlgorithm>(depth);
        }
        else if( algorithmName == "ESTreeML") {
            algorithm =
                    createSuperVertexAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::ESTreeML,false>, OverlayAlgorithm>(depth);
        }
        else if( algorithmName == "OldESTree") {
            algorithm =
                    createSuperVertexAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::OldESTree,false>, OverlayAlgorithm>(depth);
        }
        else if( algorithmName == "ESTreeQ") {
            algorithm =
                    createSuperVertexAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::ESTreeQ,false>, OverlayAlgorithm>(depth);
        }
        else if( algorithmName == "SimpleESTree") {
            algorithm =
                    createSuperVertexAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::SimpleESTree,false>, OverlayAlgorithm>(depth);
        }
        else{
            std::cerr << algorithmName << " not a viable algorithm" << std::endl;
            continue;
            //TODO throw error
        }
        //algorithm->setPartitionFunction(partitionFunction, k);
        algorithm->setK(k);
        algorithm->setRepartitionThreshold(repartitionThreshold);
        algorithms.push_back(algorithm);
    }
    return algorithms;
}

template<typename SubAlgorithm, typename OverlayAlgorithm>
PartitionedDAPReachAlgorithm *AlgorithmHandler::createSuperVertexAlgorithm(const unsigned int depth) {
    return new SuperVertexAlgorithmImplementation<SubAlgorithm, OverlayAlgorithm>(depth);
}

std::vector<DynamicAPReachAlgorithm *> *
AlgorithmHandler::createSuperVertexAlgorithms(const std::vector<std::string> &algorithmNames, unsigned long long int k,
                                              unsigned int depth, const unsigned long long int repartitionThreshold) {
    auto *algorithms = new std::vector<DynamicAPReachAlgorithm*>;

    /*std::function<Algora::FastPropertyMap<unsigned long long>(unsigned long long int, Algora::DiGraph*)>
            partitionFunction = [partitions] (unsigned long long k, Algora::DiGraph* diGraph){

                            return partitions;
            };*/


    for(const std::string& algorithmName: algorithmNames){

        PartitionedDAPReachAlgorithm *algorithm = nullptr;

        if(algorithmName == "StaticBFS") {
            algorithm =
                    createSuperVertexAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::StaticBFSSSReachAlgorithm>,Algora::StaticBFSSSReachAlgorithm>(depth);
        }
        else if(algorithmName == "StaticDFS") {
            algorithm =
                    createSuperVertexAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::StaticDFSSSReachAlgorithm>,Algora::StaticDFSSSReachAlgorithm>(depth);
        }
        else if( algorithmName == "LazyDFS") {
            algorithm =
                    createSuperVertexAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::LazyDFSSSReachAlgorithm,true>, Algora::LazyDFSSSReachAlgorithm>(depth);
        }
        else if( algorithmName == "LazyBFS") {
            algorithm =
                    createSuperVertexAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::LazyBFSSSReachAlgorithm,true>, Algora::LazyBFSSSReachAlgorithm>(depth);
        }
        else if( algorithmName == "CachingDFS") {
            algorithm =
                    createSuperVertexAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::CachingDFSSSReachAlgorithm,true>, Algora::CachingDFSSSReachAlgorithm>(depth);
        }
        else if( algorithmName == "CachingBFS") {
            algorithm =
                    createSuperVertexAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::CachingBFSSSReachAlgorithm,true>, Algora::CachingBFSSSReachAlgorithm>(depth);
        }
        else if( algorithmName == "SimpleInc") {
            algorithm =
                    createSuperVertexAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::SimpleIncSSReachAlgorithm,true>, Algora::SimpleIncSSReachAlgorithm>(depth);
        }
        else if( algorithmName == "ESTreeML") {
            algorithm =
                    createSuperVertexAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::ESTreeML,true>, Algora::ESTreeML>(depth);
        }
        else if( algorithmName == "OldESTree") {
            algorithm =
                    createSuperVertexAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::OldESTree,true>, Algora::OldESTree>(depth);
        }
        else if( algorithmName == "ESTreeQ") {
            algorithm =
                    createSuperVertexAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::ESTreeQ,true>, Algora::ESTreeQ>(depth);
        }
        else if( algorithmName == "SimpleESTree") {
            algorithm =
                    createSuperVertexAlgorithm<SSBasedDAPReachAlgorithmImplementation<Algora::SimpleESTree,true>, Algora::SimpleESTree>(depth);
        }
        else{
            std::cerr << algorithmName << " not a viable algorithm" << std::endl;
            //TODO throw error
        }
        //algorithm->setPartitionFunction(partitionFunction, k);
        algorithm->setK(k);
        algorithm->setRepartitionThreshold(repartitionThreshold);
        algorithms->push_back(algorithm);
    }
    return algorithms;
}
