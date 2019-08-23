//
// Created by Alwin Stockinger.
//

#include <algorithm.basic.traversal/breadthfirstsearch.h>
#include "BidirectionalSuperVertexPDAPReachAlgorithm.h"

void BidirectionalSuperVertexPDAPReachAlgorithm::generateSuperVertices() {
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

BidirectionalSuperVertexPDAPReachAlgorithm::~BidirectionalSuperVertexPDAPReachAlgorithm() {
    resetSuperStructure();
}


void BidirectionalSuperVertexPDAPReachAlgorithm::resetSuperStructure() {
    if(overlayGraph){
        deregisterOnOverlay();
    }

    sourceSuperVertices.resetAll();
    destinationSuperVertices.resetAll();
}

bool BidirectionalSuperVertexPDAPReachAlgorithm::query(Algora::Vertex *start, const Algora::Vertex *end) {

    if( start == end){
        return true;
    }

    if(!initialized){
        run();
    }

    start = mainToSubMap[start];
    end = mainToSubMap[end];

    if(!start || !end){
        return false;
    }

    //select subgraphs
    auto* startGraph = start->getParent();
    auto* endGraph = end->getParent();

    //same subgraph? then normal Algorithm
    if(startGraph == endGraph){
        if(graphToAlgorithmMap[startGraph] -> query(start, end)){
            return true;
        }
    }

    DynamicAPReachAlgorithm* startGraphAlgorithm = graphToAlgorithmMap[startGraph];
    DynamicAPReachAlgorithm* endGraphAlgorithm = graphToAlgorithmMap[endGraph];


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


    std::unordered_set<Algora::Vertex*> sourceNotReachableSet( sourceNotReachable.begin(), sourceNotReachable.end());
    std::unordered_set<Algora::Vertex*> destinationNotReachableSet( destinationNotReachable.begin(), destinationNotReachable.end());

    //cant use always set because have to remove and reinserted arcs have to keep order
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


    bool result = bidirectionalOverlaySearch(sourceSuperVertex, destinationSuperVertex);


    //restore Arcs, the order is important so that the correct arcs get reused.
    for (auto rit=destinationNotReachable.rbegin(); rit != destinationNotReachable.rend(); ++rit){
        overlayGraph->addArc(*(rit), destinationSuperVertex);
    }

    for (auto rit=sourceNotReachable.rbegin(); rit != sourceNotReachable.rend(); ++rit){
        overlayGraph->addArc(sourceSuperVertex, *(rit));
    }




    return result;
}



void BidirectionalSuperVertexPDAPReachAlgorithm::onArcAdd(Algora::Arc *arc) {
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

std::string BidirectionalSuperVertexPDAPReachAlgorithm::getBaseName() {
    std::string retName = "Bidirectional Super Vertex ";
    retName += "(" + getPartitionConfiguration() + ")";
    retName += "(s=" + std::to_string(stepSize) + ")";
    retName += ": " + graphToAlgorithmMap.begin()->second->getBaseName();

    return retName;
}

std::string BidirectionalSuperVertexPDAPReachAlgorithm::getName()  const noexcept {
    return std::__cxx11::string("Bidirectional Super Vertex Partitioned Algorithm");
}

std::string BidirectionalSuperVertexPDAPReachAlgorithm::getShortName() const noexcept {
    return std::__cxx11::string("Bidi Super Vertex Partitioned Algorithm");
}

void BidirectionalSuperVertexPDAPReachAlgorithm::registerOnOverlay() {
    overlayGraph->onVertexAdd(this, [this](Algora::Vertex* vertex){
        delayedVertices.push_back(vertex);
    });
}

void BidirectionalSuperVertexPDAPReachAlgorithm::deregisterOnOverlay() {
    if(overlayGraph){
        overlayGraph->removeOnVertexAdd(this);
    }
}

void BidirectionalSuperVertexPDAPReachAlgorithm::initializeChildStructures() {
    PartitionedDynamicAPReachAlgorithm::initializeChildStructures();

    generateSuperVertices();

    registerOnOverlay(); //this needs to be last, so that super vertices don't notify
}

void BidirectionalSuperVertexPDAPReachAlgorithm::resetChildStructures() {
    resetSuperStructure();
    PartitionedDynamicAPReachAlgorithm::resetChildStructures();
}

bool
BidirectionalSuperVertexPDAPReachAlgorithm::bidirectionalOverlaySearch(Algora::Vertex *source, Algora::Vertex *target) {
    bool reachable = false;

    unsigned long long stepsTaken = 0ULL;

    Algora::FastPropertyMap<bool> sourceReachable(false);
    sourceReachable[source] = true;

    Algora::FastPropertyMap<bool> targetReachable(false);
    targetReachable[target] = true;

    //std::unordered_set<const Algora::Vertex*> sourceSet = {start};
    //std::unordered_set<const Algora::Vertex*> targetSet = {end};

    Algora::BreadthFirstSearch<Algora::FastPropertyMap,false> sourceBFS(false, false);
    sourceBFS.setStartVertex(source);
    sourceBFS.setArcStopCondition([&reachable, &targetReachable](const Algora::Arc* arc){
        reachable = targetReachable(arc->getHead());
        return reachable;
    });
    sourceBFS.setVertexStopCondition([&stepsTaken, &sourceReachable, this](const Algora::Vertex* vertex){
        if(!sourceReachable(vertex)){
            sourceReachable[vertex] = true;
            stepsTaken++;
        }
        return stepsTaken == stepSize;
    });



    Algora::BreadthFirstSearch<Algora::FastPropertyMap, false> targetBFS(false, false);
    targetBFS.setStartVertex(target);
    targetBFS.reverseArcDirection(true);
    targetBFS.setArcStopCondition([&reachable, &sourceReachable](const Algora::Arc* arc){
        reachable = sourceReachable(arc->getTail());
        return reachable;
    });
    targetBFS.setVertexStopCondition([&stepsTaken,&targetReachable, this](const Algora::Vertex* vertex){
        if(!targetReachable(vertex)){
            targetReachable[vertex] = true;
            stepsTaken++;
        }
        return stepsTaken == stepSize;
    });

    runAlgorithm(sourceBFS, diGraph);
    if(reachable){
        return reachable;
    }
    else if(stepsTaken != stepSize){
        return false;
    }
    stepsTaken = 0ULL;

    runAlgorithm(targetBFS, diGraph);
    if(reachable){
        return reachable;
    }
    else if(stepsTaken != stepSize){
        return false;
    }
    stepsTaken = 0ULL;

    while(true){
        sourceBFS.resume();
        if(reachable){
            return reachable;
        }
        else if(stepsTaken != stepSize){
            return false;
        }
        stepsTaken = 0ULL;

        targetBFS.resume();
        if(reachable){
            return reachable;
        }
        else if(stepsTaken != stepSize){
            return false;
        }
        stepsTaken = 0ULL;
    }

}
