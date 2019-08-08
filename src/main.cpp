
#include <cstring>



#include "converter/GraphFileConverter.h"

#include "algorithm.reach.allPair/SimplePartitionedDAPReachAlgorithmImplementation.h"
#include "algorithm.reach.allPair/SSBasedDAPReachAlgorithmImplementation.h"
#include "handler/FactoryFacade.h"
#include "handler/AlgorithmTester.h"

#include <CLI11.hpp>
#include <instanceprovider.h>
#include <randominstanceprovider.h>
#include <konectnetworkinstanceprovider.h>
#include <spawn.h>
#include <wait.h>
#include <chrono>

typedef std::chrono::high_resolution_clock HRC;
typedef HRC::time_point TimePoint;

int main(int argc, char *argv[]) {


    CLI::App app{"Dynamic All Pairs Reachability program"};

    std::string inputFileName = "input";
    auto inputOption = app.add_option("-i, --inputFile", inputFileName, "File name of the input Graph");

    /*std::string kahipFileName = "forKahip";
    app.add_option(" --kahipFile", kahipFileName, "File name for kahip files");*/

    unsigned long long k = 10;
    app.add_option("-k, --partitions", k, "Amount of partitions");

    unsigned long long kMin = 2;
    app.add_option(" --kMin", kMin, "Minimum partitions for tests");

    bool exponentialK = false;
    app.add_option("--exponentialK", exponentialK, "K is increased exponentially for tests");

    unsigned minDepth = 0U;
    app.add_option("--minDepth", minDepth, "Minimum depth for partitions");

    unsigned maxDepth = 0U;
    app.add_option("--maxDepth", maxDepth, "Maximum depth for partitions");


    bool testWithoutPartition = false;
    app.add_option("--testWithoutPartition", testWithoutPartition, "Also test not partitioned algorithms");

    bool testPartition = true;
    app.add_option("--testPartition", testPartition, "Test Normal Partition Algorithms");

    bool testSuperVertex = false;
    app.add_option("--testSuperVertex", testSuperVertex, "Test Super Vertex Algorithms");

    bool testSimpleBFS = false;
    app.add_option("--testSBFS", testSimpleBFS, "Test Simple BFS Algorithms");

    bool testAdvancedBFS = false;
    app.add_option("--testAdvancedBFS", testAdvancedBFS, "Test Advanced BFS Algorithms");

    bool testBFSO = false;
    app.add_option("--testBFSO", testBFSO, "Test BFSO Algorithms");

    bool testReverseBFS = false;
    app.add_option("--testReverseBFS", testReverseBFS, "Test Reversed BFS Algorithms");

    std::vector<std::string> algorithmNames;
    app.add_option("-A, --algorithms", algorithmNames, "Algorithms to use");

    std::vector<std::string> overlayAlgorithms;
    app.add_option("-O, --overlayAlgorithms", overlayAlgorithms, "Overlay algorithms to use");

    unsigned long long vertexSize = 100;
    app.add_option("-s, --vertexSize", vertexSize, "Amount of vertices for random graphs");

    unsigned long long arcSize = 100;
    auto arcSizeOption = app.add_option("--arcSize", arcSize, "Amount of arcs for random graphs");

    double arcProb = 0.1;
    app.add_option("-p, --arcProbability", arcProb, "Probability of arcs for random graphs");

    bool multiArcs = true;
    app.add_option("-M, --multiArcs", multiArcs, "Multi Arcs are allowed");

    unsigned long long numOperations = 10000;
    app.add_option("-o, --numOperations", numOperations, "Number of Operations");

    unsigned queryProp = 10;
    app.add_option("-q, --queries", queryProp, "Proportion of queries");

    unsigned removalProp = 10;
    app.add_option("-r, --removalProp", removalProp, "Proportion of removals");

    unsigned additionProp = 10;
    app.add_option("-a, --additionProp", additionProp, "Proportion of additions");

    unsigned multiplier = 1;
    app.add_option("-m, --multiplier", multiplier, "Multiplier of operations");

    bool runPerformanceTests = true;
    app.add_option("-b, --performance", runPerformanceTests, "To run performance tests");

    bool detailedResults = false;
    app.add_option("--detailed", detailedResults, "Also write detailed Results");

    bool percentageTimes = false;
    app.add_option("--percentageTimes", percentageTimes, "Write timestamps for every percent to a file");

    unsigned long long timeOut = 0ULL;
    app.add_option("-t, --timeOut", timeOut, "Time out time for algorithms");

    unsigned long long repartitionThreshold = 0ULL;
    app.add_option("--repartition", repartitionThreshold, "Amount of operations until algorithms are repartitioned");

    //for konect files

    double queriesPerDelta = -1; //keep this one, so that always two vertices are there for queries
    app.add_option("-D, --queriesPerDelta", queriesPerDelta,
                   "Queries per delta to generate for konect graphs, negative numbers are relative to delta size");

    double squashRatio = 1.;
    auto squashOption = app.add_option("-S, --squash", squashRatio, "squash until arcs/vertices = squashRatio");

    CLI11_PARSE(app, argc, argv)

    Algora::InstanceProvider* provider = nullptr;

    if(!*inputOption){
        auto randomProvider = new Algora::RandomInstanceProvider;
        randomProvider->setGraphSize(vertexSize);
        if(arcSizeOption ){
            randomProvider->setInitialArcSize(arcSize);
        }
        else{
            randomProvider->setInitialArcProbability(arcProb);
        }
        randomProvider->allowMultiArcs(multiArcs);

        randomProvider->setNumOperations(numOperations);
        randomProvider->setQueriesProportion(2 * queryProp);  //TODO use new Query Generator
        randomProvider->setArcRemovalProportion(removalProp);
        randomProvider->setArcAdditionProportion(additionProp);
        randomProvider->setMultiplier(multiplier);

        randomProvider->setSeed(7969758442);    //make sure tests are reproducible

        provider = randomProvider;
    }
    else{
        auto konectProvider = new Algora::KonectNetworkInstanceProvider;
        konectProvider->addInputFile(inputFileName);


        queriesPerDelta *= 2;    //because two SS queries for one AP query
        bool relative = queriesPerDelta < 0.0;
        konectProvider->generateQueriesAfterEachDelta( relative ? -queriesPerDelta : queriesPerDelta, relative);

        konectProvider->setSeed(183788608);     //make sure tests are reproducible

        if(squashOption){
            konectProvider->squash(true);
            konectProvider->squashRatio(squashRatio);
        }
        provider = konectProvider;
    }

    std::cout << "Generating Algora Graph" << std::endl;
    provider->nextInstance();

    auto* dynGraph = provider->getGraph();
    dynGraph->resetToBigBang();
    dynGraph->applyNextDelta();



    if(runPerformanceTests){
        FactoryFacade factory(testWithoutPartition, testPartition, testBFSO, testSuperVertex, testReverseBFS);

        factory.setExponentialK(exponentialK);
        factory.setKMax(k);
        factory.setKMin(kMin);
        factory.setMinDepth(minDepth);
        factory.setMaxDepth(maxDepth);
        factory.setAlgorithmNames(algorithmNames);
        factory.setOverlayNames(overlayAlgorithms);
        factory.setRepartitionThreshold(repartitionThreshold);
        factory.setAdvancedBFSO(testAdvancedBFS);
        factory.setSimpleBFSO(testSimpleBFS);


        AlgorithmTester tester;
        std::chrono::high_resolution_clock::time_point& partitionStartTimer = tester.getPartitionStartTimer();
        std::chrono::high_resolution_clock::time_point& partitionEndTimer = tester.getPartitionEndTimer();


        PartFunc partitionFunction = [&partitionStartTimer, &partitionEndTimer](unsigned long long k, Algora::DiGraph* diGraph){
            partitionStartTimer = std::chrono::high_resolution_clock::now();
            auto partitionMap = Partitioner::handlePartitioning(k,diGraph);
            partitionEndTimer = std::chrono::high_resolution_clock::now();
            return partitionMap;
        };
        factory.setPartitionFunction(partitionFunction);

        std::vector<DynamicAPReachAlgorithm*> algorithms = factory.getAlgorithms();


        tester.setAlgorithms(algorithms);
        tester.setInstanceProvider(provider);
        tester.setTimeOut(timeOut);
        tester.setPercentageTimes(percentageTimes);
        //TODO detailed results

        tester.runTests();


    }
    else{
        //handler.runInterface();
    }

    delete provider;
}
