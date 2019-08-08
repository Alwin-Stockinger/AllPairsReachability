//
// Created by Alwin Stockinger.
//

#include "SuperVertexPDAPReachAlgorithm.h"

void SuperVertexPDAPReachAlgorithm::generateSuperVertices() {
    for(auto& [graph,_] : graphToAlgorithmMap){
        (void)(_);// unused warning

        Algora::Vertex* sourceSuperVertex = overlayGraph->addVertex();
        Algora::Vertex* destinationSuperVertex = overlayGraph->addVertex();

        std::unordered_set<Algora::Vertex*>& edgeVerticesSet = edgeVertices[graph];

        for(Algora::Vertex* edgeVertex : edgeVerticesSet){
            overlayGraph->addArc(sourceSuperVertex, mainToOverlayMap[edgeVertex]);
            overlayGraph->addArc(mainToOverlayMap[edgeVertex], destinationSuperVertex);
        }

        sourceSuperVertices[graph] = sourceSuperVertex;
        destinationSuperVertices[graph] = destinationSuperVertex;
    }
}

SuperVertexPDAPReachAlgorithm::~SuperVertexPDAPReachAlgorithm() {
    resetSuperStructure();
}

void SuperVertexPDAPReachAlgorithm::generateOverlayAlgorithms() {
    for(auto& [graph,superVertex] :sourceSuperVertices){
        auto* ssAlgorithm = createOverlayAlgorithm();
        ssAlgorithm->setGraph(overlayGraph);
        ssAlgorithm->setSource(superVertex);

        overlayAlgorithms[graph] = ssAlgorithm;
    }
}

void SuperVertexPDAPReachAlgorithm::resetSuperStructure() {

    for(auto& [graph, _]: sourceSuperVertices){
        delete overlayAlgorithms[graph];
        (void)(_);// unused warning
    }

    if(overlayGraph){
        deregisterOnOverlay();
    }

    overlayAlgorithms.resetAll();
    sourceSuperVertices.resetAll();
    destinationSuperVertices.resetAll();
}

bool SuperVertexPDAPReachAlgorithm::query(Algora::Vertex *start, const Algora::Vertex *end) {
    if(!initialized){
        run();
    }

    start = mainToSubMap[start];
    end = mainToSubMap[end];

    //select subgraphs
    auto* startGraph = start->getParent();
    auto* endGraph = end->getParent();

    //same subgraph? then normal Algorithm
    if(startGraph == endGraph){
        if(graphToAlgorithmMap[startGraph] -> query(start, end)){
            return true;
        }
    }

    for(auto& [_,algorithm] : overlayAlgorithms){
        algorithm->setAutoUpdate(false);
        (void)(_);// unused warning
    }

    DynamicAPReachAlgorithm* startGraphAlgorithm = graphToAlgorithmMap[startGraph];
    DynamicAPReachAlgorithm* endGraphAlgorithm = graphToAlgorithmMap[endGraph];

    Algora::DynamicSSReachAlgorithm* overlayAlgorithm = overlayAlgorithms[startGraph];
    overlayAlgorithm->setAutoUpdate(true);

    Algora::Vertex* sourceSuperVertex = sourceSuperVertices[startGraph];
    Algora::Vertex* destinationSuperVertex = destinationSuperVertices[endGraph];

    std::unordered_set<Algora::Vertex *>& startEdgeVertices = edgeVertices[startGraph];
    std::unordered_set<Algora::Vertex *>& endEdgeVertices = edgeVertices[endGraph];

    std::vector<Algora::Vertex*> sourceNotReachable;
    std::vector<Algora::Vertex*> destinationNotReachable;

    for (Algora::Vertex *outVertex : startEdgeVertices) {
        if (!startGraphAlgorithm->query(start, mainToSubMap[outVertex])) {
            sourceNotReachable.push_back(mainToOverlayMap[outVertex]);
        }
    }
    if(sourceNotReachable.size() == startEdgeVertices.size()){
        return false;
    }

    for(Algora::Vertex* inVertex : endEdgeVertices) {
        if (!endGraphAlgorithm->query(mainToSubMap[inVertex], end)) {
            destinationNotReachable.push_back(mainToOverlayMap[inVertex]);
        }
    }
    if(destinationNotReachable.size() == endEdgeVertices.size()){
        return false;
    }

    //cant use always set because have to remove and reinserted arcs have to keep order
    std::unordered_set<Algora::Vertex*> sourceNotReachableSet( sourceNotReachable.begin(), sourceNotReachable.end());
    std::unordered_set<Algora::Vertex*> destinationNotReachableSet( destinationNotReachable.begin(), destinationNotReachable.end());

    std::vector<Algora::Arc*> arcsToRemove;

    overlayGraph->mapOutgoingArcsUntil(sourceSuperVertex,[&sourceNotReachableSet, &arcsToRemove](Algora::Arc* arc){
        if(sourceNotReachableSet.count(arc->getHead())){
            arcsToRemove.push_back(arc);
        }
    },[&arcsToRemove, &sourceNotReachable](const Algora::Arc* arc){
        return arcsToRemove.size() == sourceNotReachable.size();
    });

    overlayGraph->mapIncomingArcsUntil(destinationSuperVertex, [&destinationNotReachableSet, &arcsToRemove](Algora::Arc* arc){
        if(destinationNotReachableSet.count(arc->getTail())){
            arcsToRemove.push_back(arc);
        }
    }, [&arcsToRemove, &sourceNotReachable, &destinationNotReachable](const Algora::Arc* arc){
        return arcsToRemove.size() == (destinationNotReachable.size() + sourceNotReachable.size());
    });

    for(Algora::Arc* arc : arcsToRemove){
        overlayGraph->removeArc(arc);
    }

    bool result = overlayAlgorithm->query(destinationSuperVertex);

    //restore Arcs, the order is important so that the correct arcs get reused.

    for (auto rit=destinationNotReachable.rbegin(); rit != destinationNotReachable.rend(); ++rit){
        overlayGraph->addArc(*(rit), destinationSuperVertex);
    }

    for (auto rit=sourceNotReachable.rbegin(); rit != sourceNotReachable.rend(); ++rit){
        overlayGraph->addArc(sourceSuperVertex, *(rit));
    }


    for(auto& [_,algorithm] : overlayAlgorithms){
        algorithm->setAutoUpdate(true);

        (void)(_); //unused warning
    }

    return result;
}


void SuperVertexPDAPReachAlgorithm::onArcAdd(Algora::Arc *arc) {
    PartitionedAPReachAlgorithm::onArcAdd(arc);

    for(Algora::Vertex* vertex: delayedVertices){
        Algora::Vertex* mainVertex;
        if(mainToOverlayMap[arc->getHead()] == vertex){
            mainVertex = arc->getHead();
        }
        else if(mainToOverlayMap[arc->getTail()] == vertex){
            mainVertex = arc->getTail();
        }
        else{
            throw std::logic_error("Delayed vertex must be head or tail");
        }

        auto* graph = mainToSubMap[mainVertex]->getParent();

        overlayGraph->addArc(sourceSuperVertices[graph], vertex);
        overlayGraph->addArc(vertex, destinationSuperVertices[graph]);
    }
    delayedVertices.clear();
}

std::string SuperVertexPDAPReachAlgorithm::getBaseName() {
    std::string retName = "Super Vertex ";
    retName += "(" + getPartitionConfiguration() + ")";
    retName += ": " + graphToAlgorithmMap.begin()->second->getBaseName();
    retName += "(" + overlayAlgorithms.begin()->second->getShortName() + ")";

    return retName;
}

std::string SuperVertexPDAPReachAlgorithm::getName()  const noexcept {
    return std::__cxx11::string("Super Vertex Partitioned Algorithm");
}

std::string SuperVertexPDAPReachAlgorithm::getShortName() const noexcept {
    return std::__cxx11::string("Super Vertex Partitioned Algorithm");
}

void SuperVertexPDAPReachAlgorithm::registerOnOverlay() {
    overlayGraph->onVertexAdd(this, [this](Algora::Vertex* vertex){
        delayedVertices.push_back(vertex);
    });
}

void SuperVertexPDAPReachAlgorithm::deregisterOnOverlay() {
    if(overlayGraph){
        overlayGraph->removeOnVertexAdd(this);
    }
}

void SuperVertexPDAPReachAlgorithm::initializeChildStructures() {
    PartitionedDynamicAPReachAlgorithm::initializeChildStructures();

    generateSuperVertices();
    generateOverlayAlgorithms();

    registerOnOverlay(); //this needs to be last, so that super vertices don't notify
}

void SuperVertexPDAPReachAlgorithm::resetChildStructures() {
    resetSuperStructure();
    PartitionedDynamicAPReachAlgorithm::resetChildStructures();
}
