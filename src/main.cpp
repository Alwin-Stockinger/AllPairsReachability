
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
    app.add_option(" -k, --partitions", k, "Amount of partitions");


    std::vector<std::string> algorithmNames;
    app.add_option("-A, --algorithms", algorithmNames, "Algorithms to use");

    unsigned long long vertexSize = 100;
    app.add_option("-s, --vertexSize", vertexSize, "Amount of vertices for random graphs");

    unsigned long long arcSize = 100;
    auto arcSizeOption = app.add_option("--arcSize", arcSize, "Amount of arcs for random graphs");

    double arcProb = 0.1;
    app.add_option("-p, --arcProbability", arcProb, "Probability of arcs for random graphs");

    bool multiArcs = false;
    auto multiArcsOption = app.add_option("-M, --multiArcs", multiArcs, "Multi Arcs are allowed");

    unsigned long long numOperations = 100000;
    auto numOperationsOption = app.add_option("-o, --numOperations", numOperations, "Number of Operations");

    unsigned queryProp = 70;
    auto queryProbOption = app.add_option("-q, --queries", queryProp, "Proportion of queries");

    unsigned removalProp = 15;
    auto removalPropOption = app.add_option("-r, --removalProp", removalProp, "Proportion of removals");

    unsigned additionProp = 15;
    auto additionPropOption = app.add_option("-a, --additionProp", additionProp, "Proportion of additions");

    unsigned multiplier = 1;
    auto multiplierOption = app.add_option("-m, --multiplier", multiplier, "Multiplier of operations");

    bool runPerformanceTests = true;
    auto runPerformanceTestsOption = app.add_option("-b, --performance", runPerformanceTests, "To run performance tests");


    //for konect files

    double queriesPerDelta = -0.5;
    auto queriesPerDeltaOption = app.add_option("-D, --queriesPerDelta", queriesPerDelta, "Queries per delta to generate for konect graphs, negative numbers are relative to delta size");

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
        handler.runTests(k, algorithmNames);
    }
    else{
        handler.runInterface();
    }

    delete provider;
}







    /*
    //TODO Error check
    std::string graphFileName = argv[1];
    std::string partitionFileName = argv[2];

    std::map<unsigned, unsigned> partitionMap = GraphFileConverter::makePartitionMap(partitionFileName);

    Algora::DynamicDiGraph
        *mainGraph = GraphFileConverter::readGraph(graphFileName);


    if(mainGraph){
        std::vector<DynamicAPReachAlgorithm*> algorithms;
        std::set<Algora::DynamicDiGraph*> overlayGraphsSet;
        std::set<std::vector<Algora::DynamicDiGraph*>*> subGraphsSet;
        std::set<std::map<const Algora::Vertex *, Algora::Vertex *>*> mapsSet;

        for(int i = 3; i < argc; i++){
            std::string algorithmName = argv[i];

            Algora::DynamicDiGraph* overlayGraph = GraphFileConverter::makeOverlay(graphFileName,partitionMap);
            overlayGraphsSet.insert(overlayGraph);

            std::vector<Algora::DynamicDiGraph*> *subGraphs = GraphFileConverter::makeSubGraphs(graphFileName,partitionMap);
            subGraphsSet.insert(subGraphs);

            std::map<const Algora::Vertex *, Algora::Vertex *>
                    * inMap = GraphFileConverter::makeInMap(overlayGraph, subGraphs, partitionMap),
                    *mainToOverlayMap = GraphFileConverter::makeMainToOverlayMap(mainGraph, overlayGraph);
            mapsSet.insert({inMap, mainToOverlayMap});



            if(algorithmName == "StaticBFS") {
                algorithms.push_back(
                        createAlgorithm<DynamicSSBasedDAPReachAlgorithm<Algora::StaticBFSSSReachAlgorithm>>(mainGraph,
                                                                                                     overlayGraph,
                                                                                                     subGraphs, inMap,
                                                                                                     mainToOverlayMap));
            }
            else if(algorithmName == "StaticDFS") {
                algorithms.push_back(
                        createAlgorithm<DynamicSSBasedDAPReachAlgorithm<Algora::StaticDFSSSReachAlgorithm>>(mainGraph,
                                                                                                     overlayGraph,
                                                                                                     subGraphs, inMap,
                                                                                                     mainToOverlayMap));
            }
            else if( algorithmName == "LazyDFS") {
                algorithms.push_back(
                        createAlgorithm<DynamicSSBasedDAPReachAlgorithm<Algora::LazyDFSSSReachAlgorithm>>(mainGraph,
                                overlayGraph,
                        subGraphs, inMap,
                        mainToOverlayMap));
            }
            else if( algorithmName == "LazyBFS") {
                algorithms.push_back(
                        createAlgorithm<DynamicSSBasedDAPReachAlgorithm<Algora::LazyBFSSSReachAlgorithm>>(
                                mainGraph,
                                overlayGraph,
                                subGraphs, inMap,
                                mainToOverlayMap));
            }
            else if( algorithmName == "CachingDFS") {
                algorithms.push_back(
                        createAlgorithm<DynamicSSBasedDAPReachAlgorithm<Algora::CachingDFSSSReachAlgorithm>>(
                                mainGraph,
                                overlayGraph,
                                subGraphs, inMap,
                                mainToOverlayMap));
            }
            else if( algorithmName == "CachingBFS") {
                algorithms.push_back(
                        createAlgorithm<DynamicSSBasedDAPReachAlgorithm<Algora::CachingBFSSSReachAlgorithm>>(
                                mainGraph,
                                overlayGraph,
                                subGraphs, inMap,
                                mainToOverlayMap));
            }
            else if( algorithmName == "SimpleInc") {
                algorithms.push_back(
                        createAlgorithm<DynamicSSBasedDAPReachAlgorithm<Algora::SimpleIncSSReachAlgorithm>>(
                                mainGraph,
                                overlayGraph,
                                subGraphs, inMap,
                                mainToOverlayMap));
            }
            else if( algorithmName == "ESTreeML") {
                algorithms.push_back(
                        createAlgorithm<DynamicSSBasedDAPReachAlgorithm<Algora::ESTreeML>>(
                                mainGraph,
                                overlayGraph,
                                subGraphs, inMap,
                                mainToOverlayMap));
            }
            else if( algorithmName == "OldESTree") {
                algorithms.push_back(
                        createAlgorithm<DynamicSSBasedDAPReachAlgorithm<Algora::OldESTree>>(
                                mainGraph,
                                overlayGraph,
                                subGraphs, inMap,
                                mainToOverlayMap));
            }
            else if( algorithmName == "ESTreeQ") {
                algorithms.push_back(
                        createAlgorithm<DynamicSSBasedDAPReachAlgorithm<Algora::ESTreeQ>>(
                                mainGraph,
                                overlayGraph,
                                subGraphs, inMap,
                                mainToOverlayMap));
            }
            else if( algorithmName == "SimpleESTree") {
                algorithms.push_back(
                        createAlgorithm<DynamicSSBasedDAPReachAlgorithm<Algora::SimpleESTree>>(
                                mainGraph,
                                overlayGraph,
                                subGraphs, inMap,
                                mainToOverlayMap));
            }
            else{
                //TODO throw error
            }
        }

        AlgorithmHandler handler(algorithms, mainGraph);
        handler.run();

        for(DynamicAPReachAlgorithm* algorithm: algorithms){
            delete algorithm;
        }
        for(Algora::DynamicDiGraph* graph: overlayGraphsSet){
            delete graph;
        }
        for(std::vector<Algora::DynamicDiGraph*>* subGraphs : subGraphsSet){
            for(Algora::DynamicDiGraph* subGraph: *subGraphs){
                delete subGraph;
            }
            delete subGraphs;
        }
        for(std::map<const Algora::Vertex *, Algora::Vertex *>* map : mapsSet){
            delete map;
        }
    }
    else{
        //TODO Error
    }
    deleteAll(mainGraph);
     */
