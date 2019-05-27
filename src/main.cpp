

#include <algorithm.reach/staticbfsssreachalgorithm.h>
#include "converter/GraphFileConverter.h"
#include "algorithm.reach.allPair/PartitionedDAPReachAlgorithm.h"
#include "handler/AlgorithmHandler.h"


DynamicAPReachAlgorithm * createAlgorithm(Algora::DynamicDiGraph *mainGraph, Algora::DynamicDiGraph *overlayGraph,
                                          std::vector<Algora::DynamicDiGraph *> *subGraphs,
                                          std::map<const Algora::Vertex *, Algora::Vertex *> *inMap,
                                          std::map<const Algora::Vertex *, Algora::Vertex *> *mainToOverlayMap);

void deleteAll(Algora::DynamicDiGraph *mainGraph, Algora::DynamicDiGraph *overlayGraph,
               std::vector<Algora::DynamicDiGraph *> *subGraphs,
               std::map<const Algora::Vertex *, Algora::Vertex *> *inMap,
               std::map<const Algora::Vertex *, Algora::Vertex *> *mainToOverlayMap);

int main(int argc, char *argv[]) {

    //TODO Error check
    std::string graphFileName = argv[1];
    std::string partitionFileName = argv[2];

    std::map<unsigned, unsigned> partitionMap = GraphFileConverter::makePartitionMap(partitionFileName);

    Algora::DynamicDiGraph
        *mainGraph = GraphFileConverter::readGraph(graphFileName),
        *overlayGraph = GraphFileConverter::makeOverlay(graphFileName,partitionMap);

    std::vector<Algora::DynamicDiGraph*> *subGraphs = GraphFileConverter::makeSubGraphs(graphFileName,partitionMap);

    std::map<const Algora::Vertex *, Algora::Vertex *>
            * inMap = GraphFileConverter::makeInMap(overlayGraph, subGraphs, partitionMap),
            *mainToOverlayMap = GraphFileConverter::makeMainToOverlayMap(mainGraph, overlayGraph);


    if(mainGraph && overlayGraph && subGraphs && inMap && mainToOverlayMap){
        DynamicAPReachAlgorithm *algorithm = createAlgorithm(mainGraph, overlayGraph, subGraphs, inMap, mainToOverlayMap);

        AlgorithmHandler handler(algorithm, mainGraph);
        handler.run();

        delete algorithm;
    }
    else{
        //TODO Error
    }

    deleteAll(mainGraph, overlayGraph, subGraphs, inMap, mainToOverlayMap);

}


DynamicAPReachAlgorithm * createAlgorithm(Algora::DynamicDiGraph *mainGraph, Algora::DynamicDiGraph *overlayGraph,
                                          std::vector<Algora::DynamicDiGraph *> *subGraphs,
                                          std::map<const Algora::Vertex *, Algora::Vertex *> *inMap,
                                          std::map<const Algora::Vertex *, Algora::Vertex *> *mainToOverlayMap) {

    std::vector<Algora::DiGraph*> subDiGraphs;

    for(Algora::DynamicDiGraph* dynamicDiGraph: *subGraphs){
        subDiGraphs.push_back(dynamicDiGraph->getDiGraph());
    }


    auto* algorithm = new PartitionedDAPReachAlgorithm<SSBasedDAPReachAlgorithm<Algora::StaticBFSSSReachAlgorithm>>();
    algorithm->setGraph(mainGraph->getDiGraph());
    algorithm->setGraphs(&subDiGraphs, overlayGraph->getDiGraph(), *inMap, *mainToOverlayMap);

    return algorithm;
}

void deleteAll(Algora::DynamicDiGraph *mainGraph, Algora::DynamicDiGraph *overlayGraph,
               std::vector<Algora::DynamicDiGraph *> *subGraphs,
               std::map<const Algora::Vertex *, Algora::Vertex *> *inMap,
               std::map<const Algora::Vertex *, Algora::Vertex *> *mainToOverlayMap) {
    delete mainGraph;
    delete overlayGraph;

    for(Algora::DynamicDiGraph* dynamicDiGraph: *subGraphs){
        delete dynamicDiGraph;
    }
    delete subGraphs;

    delete inMap;
    delete mainToOverlayMap;
}