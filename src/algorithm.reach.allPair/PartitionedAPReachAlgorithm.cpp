//
// Created by Alwin Stockinger.
//

#include "PartitionedAPReachAlgorithm.h"

#include <graph.incidencelist/incidencelistgraph.h>

void PartitionedAPReachAlgorithm::run() {

    if(initialized){
        return;
    }

    resetChildStructures();

    deleteOldPartition();

    partition();

    initializeChildStructures();

    for( Algora::DiGraph* diGraph : subGraphs){
        insertOverlayEdgeArcs(diGraph);
    }

    initialized = true;
}



PartitionedAPReachAlgorithm::~PartitionedAPReachAlgorithm() {
    deleteOldPartition();
}


void PartitionedAPReachAlgorithm::deleteOldPartition() {
    for(auto* graph: subGraphs){
        delete graph;
    }
    subGraphs.clear();

    delete overlayGraph;

    mainToOverlayMap.resetAll();
    mainToSubMap.resetAll();

    edgeVertices.resetAll();
}





void PartitionedAPReachAlgorithm::onVertexAdd(Algora::Vertex *vertex) {

    if(!initialized){
        return;
    }

    if(repartitionThreshold && ++operationsSinceRepartition >= repartitionThreshold){
        initialized = false;
    } else if(randomVertexAdditions){
        auto* subGraph = subGraphs[kDistribution(randomGenerator)];
        addVertex(vertex, subGraph);
    }

    DynamicAPReachAlgorithm::onVertexAdd(vertex);
}

void PartitionedAPReachAlgorithm::addNeighbourVertex(Algora::Vertex *lazyVertex, Algora::Vertex *neighbourVertex) {
    auto* neighbourGraph = dynamic_cast<Algora::DiGraph*>(mainToSubMap[neighbourVertex]->getParent());
    addVertex(lazyVertex, neighbourGraph);
}

void PartitionedAPReachAlgorithm::addLazyVertexPair(Algora::Vertex *lazyHead, Algora::Vertex *lazyTail) {
    auto* subGraph = subGraphs[kDistribution(randomGenerator)];

    addVertex(lazyHead, subGraph);
    addVertex(lazyTail, subGraph);
}

void PartitionedAPReachAlgorithm::addVertex(Algora::Vertex *lazyVertex, Algora::DiGraph *subGraph) {
    mainToSubMap[lazyVertex] = subGraph->addVertex();
}



void PartitionedAPReachAlgorithm::onVertexRemove(Algora::Vertex *vertex) {

    if(!initialized){
        return;
    }

    if(repartitionThreshold && ++operationsSinceRepartition >= repartitionThreshold){
        initialized = false;
    } else {

        Algora::Vertex *subVertex = mainToSubMap[vertex];
        if(!subVertex){
            return;
        }

        Algora::Vertex *overlayVertex = mainToOverlayMap[vertex];

        auto * subGraph = dynamic_cast<Algora::DiGraph*>(subVertex->getParent());
        subGraph->removeVertex(subVertex);
        mainToSubMap.resetToDefault(vertex);

        if(overlayVertex) {
            overlayGraph->removeVertex(overlayVertex);
            mainToOverlayMap.resetToDefault(vertex);
            edgeVertices[subGraph].erase(vertex);
        }
    }
    DynamicAPReachAlgorithm::onVertexRemove(vertex);
}


void PartitionedAPReachAlgorithm::onArcAdd(Algora::Arc *arc) {

    if(!initialized){
        return;
    }

    DynamicAPReachAlgorithm::onArcAdd(arc);

    if(repartitionThreshold && ++operationsSinceRepartition >= repartitionThreshold){
        initialized = false;
    } else {
        auto *mainHead = arc->getHead();
        auto *mainTail = arc->getTail();

        auto *subHead = mainToSubMap[mainHead];
        auto *subTail = mainToSubMap[mainTail];

        bool delayedVertex = false;

        if( !subHead && !subTail){
            addLazyVertexPair(mainHead, mainTail);
            subHead = mainToSubMap[mainHead];
            subTail = mainToSubMap[mainTail];
            delayedVertex = true;
        }
        else if(!subHead){
            addNeighbourVertex(mainHead, mainTail);
            subHead = mainToSubMap[mainHead];
            delayedVertex = true;
        }
        else if(!subTail){
            addNeighbourVertex(mainTail, mainHead);
            subTail = mainToSubMap[mainTail];
            delayedVertex = true;
        }

        auto *headGraph = subHead->getParent();
        auto *tailGraph = subTail->getParent();

        if (headGraph == tailGraph) {
            auto *subGraph = dynamic_cast<Algora::DiGraph *>(headGraph);
            subGraph->addArc(subTail, subHead);

            if(!delayedVertex){
                insertOverlayEdgeArcs(subGraph);
            }
        } else {
            auto *overlayHead = mainToOverlayMap[mainHead];
            auto *overlayTail = mainToOverlayMap[mainTail];

            if (!overlayHead) {
                overlayHead = overlayGraph->addVertex();
                mainToOverlayMap[mainHead] = overlayHead;
                edgeVertices[headGraph].insert(mainHead);
                insertOverlayEdgeArcsOfNewOverlayVertex(mainHead);
            }
            if (!overlayTail) {
                overlayTail = overlayGraph->addVertex();
                mainToOverlayMap[mainTail] = overlayTail;
                edgeVertices[tailGraph].insert(mainTail);
                insertOverlayEdgeArcsOfNewOverlayVertex(mainTail);
            }

            overlayGraph->addArc(overlayTail, overlayHead);
        }
    }
}


void PartitionedAPReachAlgorithm::onArcRemove(Algora::Arc *arc) {

    if(!initialized){
        return;
    }

    DynamicAPReachAlgorithm::onArcRemove(arc);


    if(repartitionThreshold && ++operationsSinceRepartition >= repartitionThreshold){
        initialized = false;
    } else {

        auto *mainHead = arc->getHead();
        auto *mainTail = arc->getTail();

        auto *subHead = mainToSubMap[mainHead];
        auto *subTail = mainToSubMap[mainTail];

        auto *headGraph = subHead->getParent();
        auto *tailGraph = subTail->getParent();

        if (headGraph == tailGraph) {
            auto *subGraph = dynamic_cast<Algora::DiGraph *>(headGraph);
            Algora::Arc *subArc = subGraph->findArc(subTail, subHead);
            subGraph->removeArc(subArc);

            removeOverlayEdgeArcs(subGraph);

        } else {
            auto *overlayHead = mainToOverlayMap[mainHead];
            auto *overlayTail = mainToOverlayMap[mainTail];

            Algora::Arc *overlayArc = overlayGraph->findArc(overlayTail, overlayHead);
            overlayGraph->removeArc(overlayArc);

            //check if head can be removed from overlay
            if(checkIfOverlayIsolated(mainHead)){
                removeOverlayVertex(mainHead);
            }

            //check if tail can be removed from overlay
            if(checkIfOverlayIsolated(mainTail)){
                removeOverlayVertex(mainTail);
            }
        }
    }
}


std::vector<Algora::DiGraph*>
PartitionedAPReachAlgorithm::partition() {
    const Algora::FastPropertyMap<unsigned long long> &partitionMap = partitionFunction(k,diGraph);


    for(auto i = 0ULL; i < k; i++){
        auto* graph = new Algora::IncidenceListGraph;

        graph->setParent(diGraph);

        subGraphs.push_back(graph);
    }
    overlayGraph = new Algora::IncidenceListGraph;
    overlayGraph->setParent(diGraph);



    diGraph->mapVertices([ this, partitionMap](Algora::Vertex* vertex){
        auto* subVertex = subGraphs.at(partitionMap.getValue(vertex))->addVertex();
        mainToSubMap[vertex] = subVertex;
    });

    diGraph->mapArcs( [this, partitionMap](Algora::Arc* arc){

        auto* mainHead = arc->getHead();
        auto * mainTail = arc->getTail();

        unsigned long long headPartition = partitionMap.getValue(mainHead);
        unsigned long long tailPartition = partitionMap.getValue(mainTail);

        Algora::Vertex* subTail = mainToSubMap[mainTail];
        Algora::Vertex* subHead = mainToSubMap[mainHead];


        if( headPartition == tailPartition){
            subGraphs.at(headPartition)->addArc(subTail, subHead);
        }
        else{
            auto* overlayTail = mainToOverlayMap[mainTail];
            auto* overlayHead = mainToOverlayMap[mainHead];

            if(!overlayTail){
                overlayTail = overlayGraph->addVertex();
                mainToOverlayMap[mainTail] = overlayTail;
                edgeVertices[subTail->getParent()].insert(mainTail);
            }
            if(!overlayHead){
                overlayHead = overlayGraph->addVertex();
                mainToOverlayMap[mainHead] = overlayHead;
                edgeVertices[subHead->getParent()].insert(mainHead);
            }
            overlayGraph->addArc(overlayTail, overlayHead);
        }
    });


    operationsSinceRepartition = 0;

    return subGraphs;
}


void PartitionedAPReachAlgorithm::onDiGraphSet() {
    DynamicDiGraphAlgorithm::onDiGraphSet();

    initialized = false;
}


void PartitionedAPReachAlgorithm::onDiGraphUnset() {
    DynamicDiGraphAlgorithm::onDiGraphUnset();

    initialized = false;
}



bool PartitionedAPReachAlgorithm::checkIfOverlayIsolated(Algora::Vertex *vertex) {

    bool overlayIsolated = true;

    auto* subGraph = mainToSubMap[vertex]->getParent();

    diGraph->mapOutgoingArcsUntil(vertex, [this, &overlayIsolated, &subGraph] (Algora::Arc* arc){
        overlayIsolated = mainToSubMap[arc->getHead()]->getParent() == subGraph;
    }, [&overlayIsolated](const Algora::Arc* arc){
        return !overlayIsolated;
    });
    diGraph->mapIncomingArcsUntil(vertex, [this, &overlayIsolated, &subGraph] (Algora::Arc* arc){
        overlayIsolated = mainToSubMap[arc->getTail()]->getParent() == subGraph;
    }, [&overlayIsolated](const Algora::Arc* arc){
        return !overlayIsolated;
    });

    return overlayIsolated;
}

void PartitionedAPReachAlgorithm::removeOverlayVertex(Algora::Vertex *vertex) {

    auto* subGraph = mainToSubMap[vertex]->getParent();

    edgeVertices[subGraph].erase(vertex);
    overlayGraph->removeVertex(mainToOverlayMap[vertex]);
    mainToOverlayMap.resetToDefault(vertex);
}

