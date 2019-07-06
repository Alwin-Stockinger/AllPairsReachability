
#include <cstring>



#include "converter/GraphFileConverter.h"

#include "algorithm.reach.allPair/PartitionedDAPReachAlgorithmImplementation.h"
#include "algorithm.reach.allPair/SSBasedDAPReachAlgorithmImplementation.h"
#include "handler/AlgorithmHandler.h"

#include <CLI11.hpp>
#include <instanceprovider.h>
#include <randominstanceprovider.h>
#include <konectnetworkinstanceprovider.h>
#include <spawn.h>
#include <wait.h>




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

    bool withoutPartition = false;
    app.add_option("--withoutPartition", withoutPartition, "Also test not partitioned algorithms");


    std::vector<std::string> algorithmNames;
    app.add_option("-A, --algorithms", algorithmNames, "Algorithms to use");

    unsigned long long vertexSize = 100;
    app.add_option("-s, --vertexSize", vertexSize, "Amount of vertices for random graphs");

    unsigned long long arcSize = 100;
    auto arcSizeOption = app.add_option("--arcSize", arcSize, "Amount of arcs for random graphs");

    double arcProb = 0.1;
    app.add_option("-p, --arcProbability", arcProb, "Probability of arcs for random graphs");

    bool multiArcs = false;
    app.add_option("-M, --multiArcs", multiArcs, "Multi Arcs are allowed");

    unsigned long long numOperations = 10000;
    app.add_option("-o, --numOperations", numOperations, "Number of Operations");

    unsigned queryProp = 70;
    app.add_option("-q, --queries", queryProp, "Proportion of queries");

    unsigned removalProp = 15;
    app.add_option("-r, --removalProp", removalProp, "Proportion of removals");

    unsigned additionProp = 15;
    app.add_option("-a, --additionProp", additionProp, "Proportion of additions");

    unsigned multiplier = 1;
    app.add_option("-m, --multiplier", multiplier, "Multiplier of operations");

    bool runPerformanceTests = true;
    app.add_option("-b, --performance", runPerformanceTests, "To run performance tests");

    bool detailedResults = false;
    app.add_option("--detailed", detailedResults, "Also write detailed Results");

    unsigned long long timeOut = 0;
    app.add_option("-t, --timeOut", timeOut, "Time out time for algorithms");


    //for konect files

    double queriesPerDelta = -0.5;
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

        provider = randomProvider;
    }
    else{
        auto konectProvider = new Algora::KonectNetworkInstanceProvider;
        konectProvider->addInputFile(inputFileName);


        queriesPerDelta *=2;    //because two SS queries for one AP query
        bool relative = queriesPerDelta < 0.0;
        konectProvider->generateQueriesAfterEachDelta( relative ? -queriesPerDelta : queriesPerDelta, relative);

        if(squashOption){
            konectProvider->squash(true);
            konectProvider->squashRatio(squashRatio);
        }

        provider = konectProvider;
    }

    std::cout << "Generating Algora Graph" << std::endl;
    provider->nextInstance();

    auto& dynGraph = provider->getGraph();
    dynGraph.resetToBigBang();
    dynGraph.applyNextDelta();




    AlgorithmHandler handler(provider);


    if(runPerformanceTests){
        handler.runTests(algorithmNames, exponentialK, k, kMin, timeOut, detailedResults, minDepth, maxDepth, withoutPartition);
    }
    else{
        handler.runInterface();
    }

    delete provider;
}