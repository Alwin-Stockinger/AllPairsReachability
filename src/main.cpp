

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


template<typename T> DynamicAPReachAlgorithm * createAlgorithm(Algora::DynamicDiGraph *mainGraph, Algora::DynamicDiGraph *overlayGraph,
                                          std::vector<Algora::DynamicDiGraph *> *subGraphs,
                                          std::map<const Algora::Vertex *, Algora::Vertex *> *inMap,
                                          std::map<const Algora::Vertex *, Algora::Vertex *> *mainToOverlayMap);

void deleteAll(Algora::DynamicDiGraph *mainGraph);

int main(int argc, char *argv[]) {

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
}


template<typename T> DynamicAPReachAlgorithm * createAlgorithm(Algora::DynamicDiGraph *mainGraph, Algora::DynamicDiGraph *overlayGraph,
                                          std::vector<Algora::DynamicDiGraph *> *subGraphs,
                                          std::map<const Algora::Vertex *, Algora::Vertex *> *inMap,
                                          std::map<const Algora::Vertex *, Algora::Vertex *> *mainToOverlayMap) {
    std::vector<Algora::DiGraph*> subDiGraphs;

    for(Algora::DynamicDiGraph* dynamicDiGraph: *subGraphs){
        subDiGraphs.push_back(dynamicDiGraph->getDiGraph());
    }


    auto* algorithm = new PartitionedDAPReachAlgorithm<T>();
    algorithm->setGraph(mainGraph->getDiGraph());
    algorithm->setGraphs(&subDiGraphs, overlayGraph->getDiGraph(), *inMap, *mainToOverlayMap);

    return algorithm;
}

void deleteAll(Algora::DynamicDiGraph *mainGraph) {
    delete mainGraph;
}