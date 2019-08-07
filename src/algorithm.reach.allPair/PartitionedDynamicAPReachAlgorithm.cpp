//
// Created by Alwin Stockinger.
//

#include "PartitionedDynamicAPReachAlgorithm.h"




void
PartitionedDynamicAPReachAlgorithm::initializeChildStructures() {

    //create new Algorithms
    for (Algora::DiGraph *graph : subGraphs) {
        DynamicAPReachAlgorithm* algorithm = createSubAlgorithm();
        algorithm -> setGraph(graph);
        graphToAlgorithmMap.setValue(graph, algorithm);
    }
}

void PartitionedDynamicAPReachAlgorithm::insertOverlayEdgeArcsOfVertex(Algora::Vertex *vertex) {

    auto* overlayVertex = mainToOverlayMap[vertex];
    auto* subVertex = mainToSubMap[vertex];
    auto* subGraph = subVertex->getParent();
    auto* subGraphAlgorithm = graphToAlgorithmMap[subGraph];
    auto& edges = edgeVertices[subGraph];


    for(Algora::Vertex* edge: edges){
        if(subGraphAlgorithm->query(subVertex, mainToSubMap[edge])){
            overlayGraph->addArc(overlayVertex, mainToOverlayMap[edge]);
        }
        if(subGraphAlgorithm->query(mainToSubMap[edge], subVertex)){
            overlayGraph->addArc(mainToOverlayMap[edge], overlayVertex);
        }
    }

}

void PartitionedDynamicAPReachAlgorithm::insertOverlayEdgeArcs(Algora::DiGraph *subGraph) {
    std::unordered_set<Algora::Vertex*>& edges = edgeVertices[subGraph];
    DynamicAPReachAlgorithm* subAlgorithm = graphToAlgorithmMap[subGraph];

    for(Algora::Vertex* source : edges){

        Algora::Vertex* subSource = mainToSubMap[source];
        Algora::Vertex* overlaySource = mainToOverlayMap[source];

        std::unordered_set<Algora::Vertex*> newReachableEdges{};

        for(Algora::Vertex* destination : edges){
            if( source != destination){

                Algora::Vertex* subDestination = mainToSubMap[destination];

                //vertices are already in overlay because they are edge vertices!

                Algora::Vertex* overlayDestination = mainToOverlayMap[destination];

                if(subAlgorithm->query(subSource, subDestination)){
                    newReachableEdges.insert(overlayDestination);
                }
            }
        }

        overlayGraph->mapOutgoingArcsUntil(overlaySource,[&newReachableEdges](Algora::Arc* arc){
            newReachableEdges.erase(arc->getHead());
        }, [&newReachableEdges](const Algora::Arc* arc){
            return newReachableEdges.empty();
        });

        for(Algora::Vertex* vertex: newReachableEdges){
            overlayGraph->addArc(overlaySource, vertex);
        }
    }
}




void PartitionedDynamicAPReachAlgorithm::removeOverlayEdgeArcs(Algora::DiGraph *subGraph) {
    std::unordered_set<Algora::Vertex*>& edges = edgeVertices[subGraph];
    DynamicAPReachAlgorithm* subAlgorithm = graphToAlgorithmMap[subGraph];

    for(Algora::Vertex* source : edges){
        Algora::Vertex* subSource = mainToSubMap[source];
        Algora::Vertex* overlaySource = mainToOverlayMap[source];

        std::unordered_set<Algora::Vertex*> unreachableVertices;

        for(Algora::Vertex* destination : edges) {
            if (source != destination) {

                Algora::Vertex *subDestination = mainToSubMap[destination];
                Algora::Vertex *overlayDestination = mainToOverlayMap[destination];

                if (!subAlgorithm->query(subSource, subDestination)) {
                    unreachableVertices.insert(overlayDestination);
                }
            }
        }

        std::vector<Algora::Arc*> arcsToRemove{};
        overlayGraph->mapOutgoingArcsUntil(overlaySource, [&unreachableVertices, &arcsToRemove](Algora::Arc* arc){
            if(unreachableVertices.count(arc->getHead())){
                arcsToRemove.push_back(arc);
            }
        },[&unreachableVertices, &arcsToRemove](const Algora::Arc* arc){
            return unreachableVertices.size() == arcsToRemove.size();
        });

        for(Algora::Arc* arc : arcsToRemove){
            overlayGraph->removeArc(arc);
        }
    }
}

void PartitionedDynamicAPReachAlgorithm::resetChildStructures() {
    for(auto& [_,algorithm] : graphToAlgorithmMap){
        delete algorithm;
    }

    graphToAlgorithmMap.resetAll();
}


