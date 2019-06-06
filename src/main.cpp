

#include <algorithm.reach/staticbfsssreachalgorithm.h>
#include <cstring>
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
#include "converter/GraphFileConverter.h"
#include "algorithm.reach.allPair/PartitionedDAPReachAlgorithm.h"
#include "handler/AlgorithmHandler.h"

#include <CLI11.hpp>
#include <instanceprovider.h>
#include <randominstanceprovider.h>
#include <konectnetworkinstanceprovider.h>


template<typename T> DynamicAPReachAlgorithm * createAlgorithm(const Algora::DynamicDiGraph& mainGraph, Algora::DynamicDiGraph *overlayGraph,
                                          std::vector<Algora::DynamicDiGraph *> *subGraphs,
                                          std::map<const Algora::Vertex *, Algora::Vertex *> *inMap,
                                          std::map<const Algora::Vertex *, Algora::Vertex *> *mainToOverlayMap);

void deleteAll(Algora::DynamicDiGraph *mainGraph);

int main(int argc, char *argv[]) {


    CLI::App app{"Dynamic All Pairs Reachability program"};

    std::string graphType = "random";
    auto typeOption = app.add_option("-t, --type", graphType, "Type of input Graph(e.g. random, konect)");

    std::string inputFileName = "input";
    app.add_option("-i, --inputFile", inputFileName, "File name of the input Graph");

    std::string kahipFileName = "forKahip";
    app.add_option("-k, --kahipFile", kahipFileName, "File name for kahip files");

    std::vector<std::string> algorithmNames;
    app.add_option("-A, --algorithms", algorithmNames, "Algorithms to use");

    unsigned long long vertexSize = 100;
    app.add_option("-s, --vertexSize", vertexSize, "Amount of vertices for random graphs");

    unsigned long long arcSize = 100;
    app.add_option("--arcSize", arcSize, "Amount of arcs for random graphs");

    double arcProb = 0.1;
    app.add_option("-p, --arcProbability", arcProb, "Probability of arcs for random graphs");

    bool multiArcs = false;
    auto multiArcsOption = app.add_option("-M, --multiArcs", multiArcs, "Multi Arcs are allowed");

    unsigned long long numOperations = 100;
    auto numOperationsOption = app.add_option("-o, --numOperations", numOperations, "Number of Operations");

    unsigned queryProp = 70;
    auto queryProbOption = app.add_option("-q, --queryProp", queryProp, "Proportion of queries");

    unsigned removalProp = 15;
    auto removalPropOption = app.add_option("-r, --removalProp", removalProp, "Proportion of removals");

    unsigned additionProp = 15;
    auto additionPropOption = app.add_option("-a, --additionProp", additionProp, "Proportion of additions");

    unsigned multiplier = 1;
    auto multiplierOption = app.add_option("-m, --multiplier", multiplier, "Multiplier of operations");


    CLI11_PARSE(app, argc, argv)

    Algora::InstanceProvider* provider = nullptr;

    if(graphType == "random"){
        auto randomProvider = new Algora::RandomInstanceProvider;
        randomProvider->setGraphSize(vertexSize);
        randomProvider->setInitialArcProbability(arcProb);
        randomProvider->setInitialArcSize(arcSize);
        randomProvider->allowMultiArcs(multiArcs);

        randomProvider->setNumOperations(numOperations);
        randomProvider->setQueriesProportion(queryProp);
        randomProvider->setArcRemovalProportion(removalProp);
        randomProvider->setArcAdditionProportion(additionProp);
        randomProvider->setMultiplier(multiplier);

        provider = randomProvider;
    }
    else if(graphType == "konect"){
        auto konectProvider = new Algora::KonectNetworkInstanceProvider;
        konectProvider->addInputFile(inputFileName);

        provider = konectProvider;
    }
    else{
        //TODO error

        std::cerr << graphType << " is not a viable graphtype\n";
        return 1;
    }

    provider->nextInstance();

    auto& dynGraph = provider->getGraph();
    dynGraph.resetToBigBang();
    dynGraph.applyNextDelta();

    GraphFileConverter::convertDiGraphToKahip(dynGraph.getDiGraph(), kahipFileName);

    std::string kahipInputFileName;

    std::cout << "Kahip input file name:\n";
    std::cin >> kahipInputFileName;


    std::vector<DynamicAPReachAlgorithm*> algorithms;
    std::set<Algora::DynamicDiGraph*> overlayGraphsSet;
    std::set<std::vector<Algora::DynamicDiGraph*>*> subGraphsSet;
    std::set<std::map<const Algora::Vertex *, Algora::Vertex *>*> mapsSet;

    std::map<unsigned long long, unsigned long long > partitionMap = GraphFileConverter::makePartitionMap(kahipInputFileName);


    for(const std::string& algorithmName: algorithmNames){
        Algora::DynamicDiGraph* overlayGraph = GraphFileConverter::makeOverlay(dynGraph,partitionMap);
        overlayGraphsSet.insert(overlayGraph);

        std::vector<Algora::DynamicDiGraph*> *subGraphs = GraphFileConverter::makeSubGraphs(dynGraph,partitionMap);
        subGraphsSet.insert(subGraphs);

        std::map<const Algora::Vertex *, Algora::Vertex *>
                * inMap = GraphFileConverter::makeInMap(overlayGraph, subGraphs, partitionMap),
                *mainToOverlayMap = GraphFileConverter::makeMainToOverlayMap(dynGraph, overlayGraph);
        mapsSet.insert({mainToOverlayMap, inMap});

        if(algorithmName == "StaticBFS") {
            algorithms.push_back(
                    createAlgorithm<SSBasedDAPReachAlgorithm<Algora::StaticBFSSSReachAlgorithm>>(dynGraph,
                                                                                                 overlayGraph,
                                                                                                 subGraphs, inMap,
                                                                                                 mainToOverlayMap));
        }
        else if(algorithmName == "StaticDFS") {
            algorithms.push_back(
                    createAlgorithm<SSBasedDAPReachAlgorithm<Algora::StaticDFSSSReachAlgorithm>>(dynGraph,
                                                                                                 overlayGraph,
                                                                                                 subGraphs, inMap,
                                                                                                 mainToOverlayMap));
        }
        else if( algorithmName == "LazyDFS") {
            algorithms.push_back(
                    createAlgorithm<SSBasedDAPReachAlgorithm<Algora::LazyDFSSSReachAlgorithm>>(dynGraph,
                                                                                               overlayGraph,
                                                                                               subGraphs, inMap,
                                                                                               mainToOverlayMap));
        }
        else if( algorithmName == "LazyBFS") {
            algorithms.push_back(
                    createAlgorithm<SSBasedDAPReachAlgorithm<Algora::LazyBFSSSReachAlgorithm>>(
                            dynGraph,
                            overlayGraph,
                            subGraphs, inMap,
                            mainToOverlayMap));
        }
        else if( algorithmName == "CachingDFS") {
            algorithms.push_back(
                    createAlgorithm<SSBasedDAPReachAlgorithm<Algora::CachingDFSSSReachAlgorithm>>(
                            dynGraph,
                            overlayGraph,
                            subGraphs, inMap,
                            mainToOverlayMap));
        }
        else if( algorithmName == "CachingBFS") {
            algorithms.push_back(
                    createAlgorithm<SSBasedDAPReachAlgorithm<Algora::CachingBFSSSReachAlgorithm>>(
                            dynGraph,
                            overlayGraph,
                            subGraphs, inMap,
                            mainToOverlayMap));
        }
        else if( algorithmName == "SimpleInc") {
            algorithms.push_back(
                    createAlgorithm<SSBasedDAPReachAlgorithm<Algora::SimpleIncSSReachAlgorithm>>(
                            dynGraph,
                            overlayGraph,
                            subGraphs, inMap,
                            mainToOverlayMap));
        }
        else if( algorithmName == "ESTreeML") {
            algorithms.push_back(
                    createAlgorithm<SSBasedDAPReachAlgorithm<Algora::ESTreeML>>(
                            dynGraph,
                            overlayGraph,
                            subGraphs, inMap,
                            mainToOverlayMap));
        }
        else if( algorithmName == "OldESTree") {
            algorithms.push_back(
                    createAlgorithm<SSBasedDAPReachAlgorithm<Algora::OldESTree>>(
                            dynGraph,
                            overlayGraph,
                            subGraphs, inMap,
                            mainToOverlayMap));
        }
        else if( algorithmName == "ESTreeQ") {
            algorithms.push_back(
                    createAlgorithm<SSBasedDAPReachAlgorithm<Algora::ESTreeQ>>(
                            dynGraph,
                            overlayGraph,
                            subGraphs, inMap,
                            mainToOverlayMap));
        }
        else if( algorithmName == "SimpleESTree") {
            algorithms.push_back(
                    createAlgorithm<SSBasedDAPReachAlgorithm<Algora::SimpleESTree>>(
                            dynGraph,
                            overlayGraph,
                            subGraphs, inMap,
                            mainToOverlayMap));
        }
        else{
            std::cerr << algorithmName << " not a viable algorithm" << std::endl;
            //TODO throw error
        }
    }
    AlgorithmHandler handler(algorithms, &dynGraph);
    handler.runInterface();

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
                        createAlgorithm<SSBasedDAPReachAlgorithm<Algora::StaticBFSSSReachAlgorithm>>(mainGraph,
                                                                                                     overlayGraph,
                                                                                                     subGraphs, inMap,
                                                                                                     mainToOverlayMap));
            }
            else if(algorithmName == "StaticDFS") {
                algorithms.push_back(
                        createAlgorithm<SSBasedDAPReachAlgorithm<Algora::StaticDFSSSReachAlgorithm>>(mainGraph,
                                                                                                     overlayGraph,
                                                                                                     subGraphs, inMap,
                                                                                                     mainToOverlayMap));
            }
            else if( algorithmName == "LazyDFS") {
                algorithms.push_back(
                        createAlgorithm<SSBasedDAPReachAlgorithm<Algora::LazyDFSSSReachAlgorithm>>(mainGraph,
                                overlayGraph,
                        subGraphs, inMap,
                        mainToOverlayMap));
            }
            else if( algorithmName == "LazyBFS") {
                algorithms.push_back(
                        createAlgorithm<SSBasedDAPReachAlgorithm<Algora::LazyBFSSSReachAlgorithm>>(
                                mainGraph,
                                overlayGraph,
                                subGraphs, inMap,
                                mainToOverlayMap));
            }
            else if( algorithmName == "CachingDFS") {
                algorithms.push_back(
                        createAlgorithm<SSBasedDAPReachAlgorithm<Algora::CachingDFSSSReachAlgorithm>>(
                                mainGraph,
                                overlayGraph,
                                subGraphs, inMap,
                                mainToOverlayMap));
            }
            else if( algorithmName == "CachingBFS") {
                algorithms.push_back(
                        createAlgorithm<SSBasedDAPReachAlgorithm<Algora::CachingBFSSSReachAlgorithm>>(
                                mainGraph,
                                overlayGraph,
                                subGraphs, inMap,
                                mainToOverlayMap));
            }
            else if( algorithmName == "SimpleInc") {
                algorithms.push_back(
                        createAlgorithm<SSBasedDAPReachAlgorithm<Algora::SimpleIncSSReachAlgorithm>>(
                                mainGraph,
                                overlayGraph,
                                subGraphs, inMap,
                                mainToOverlayMap));
            }
            else if( algorithmName == "ESTreeML") {
                algorithms.push_back(
                        createAlgorithm<SSBasedDAPReachAlgorithm<Algora::ESTreeML>>(
                                mainGraph,
                                overlayGraph,
                                subGraphs, inMap,
                                mainToOverlayMap));
            }
            else if( algorithmName == "OldESTree") {
                algorithms.push_back(
                        createAlgorithm<SSBasedDAPReachAlgorithm<Algora::OldESTree>>(
                                mainGraph,
                                overlayGraph,
                                subGraphs, inMap,
                                mainToOverlayMap));
            }
            else if( algorithmName == "ESTreeQ") {
                algorithms.push_back(
                        createAlgorithm<SSBasedDAPReachAlgorithm<Algora::ESTreeQ>>(
                                mainGraph,
                                overlayGraph,
                                subGraphs, inMap,
                                mainToOverlayMap));
            }
            else if( algorithmName == "SimpleESTree") {
                algorithms.push_back(
                        createAlgorithm<SSBasedDAPReachAlgorithm<Algora::SimpleESTree>>(
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


template<typename T> DynamicAPReachAlgorithm * createAlgorithm(const Algora::DynamicDiGraph &mainGraph, Algora::DynamicDiGraph *overlayGraph,
                                          std::vector<Algora::DynamicDiGraph *> *subGraphs,
                                          std::map<const Algora::Vertex *, Algora::Vertex *> *inMap,
                                          std::map<const Algora::Vertex *, Algora::Vertex *> *mainToOverlayMap) {
    std::vector<Algora::DiGraph*> subDiGraphs;

    for(Algora::DynamicDiGraph* dynamicDiGraph: *subGraphs){
        subDiGraphs.push_back(dynamicDiGraph->getDiGraph());
    }


    auto* algorithm = new PartitionedDAPReachAlgorithm<T>();
    algorithm->setGraph(mainGraph.getDiGraph());
    algorithm->setGraphs(&subDiGraphs, overlayGraph->getDiGraph(), *inMap, *mainToOverlayMap);

    return algorithm;
}

void deleteAll(Algora::DynamicDiGraph *mainGraph) {
    delete mainGraph;
}