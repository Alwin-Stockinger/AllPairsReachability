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
        return graphToAlgorithmMap[startGraph] -> query(start, end);
    }
    else {

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
        for(Algora::Vertex* inVertex : endEdgeVertices) {
            if (!endGraphAlgorithm->query(mainToSubMap[inVertex], end)) {
                destinationNotReachable.push_back(mainToOverlayMap[inVertex]);
            }
        }

        for(Algora::Vertex* notReachableVertex : sourceNotReachable){
            Algora::Arc* arcToRemove = overlayGraph->findArc(sourceSuperVertex, notReachableVertex);
            overlayGraph->removeArc(arcToRemove);
        }

        for(Algora::Vertex* notReachableVertex : destinationNotReachable){
            Algora::Arc* arcToRemove = overlayGraph->findArc(notReachableVertex, destinationSuperVertex);
            overlayGraph->removeArc(arcToRemove);
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
}


void SuperVertexPDAPReachAlgorithm::onArcAdd(Algora::Arc *arc) {
    PartitionedDAPReachAlgorithm::onArcAdd(arc);

    for(Algora::Vertex* vertex: delayedVertices){
        Algora::Vertex* mainVertex;
        if(mainToOverlayMap[arc->getHead()] == vertex){
            mainVertex = arc->getHead();
        }
        else if(mainToOverlayMap[arc->getTail()] == vertex){
            mainVertex = arc->getTail();
        }
        else{
            throw std::logic_error("Delayed Vertex must be head or Tail");
        }

        auto* graph = mainToSubMap[mainVertex]->getParent();

        overlayGraph->addArc(sourceSuperVertices[graph], vertex);
        overlayGraph->addArc(vertex, destinationSuperVertices[graph]);
    }
    delayedVertices.clear();
}

std::string SuperVertexPDAPReachAlgorithm::getBaseName() {

    std::string subName = graphToAlgorithmMap.begin()->second->getBaseName();
    std::string overlayName = overlayAlgorithms.begin()->second->getShortName();

    return "Super Vertex: " + subName + " (" + overlayName + ")";

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
    overlayGraph->removeOnVertexAdd(this);
}
