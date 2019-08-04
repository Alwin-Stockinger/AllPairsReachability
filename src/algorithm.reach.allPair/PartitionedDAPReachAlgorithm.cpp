//
// Created by Alwin Stockinger.
//

#include "PartitionedDAPReachAlgorithm.h"

#include <graph.incidencelist/incidencelistgraph.h>

void PartitionedDAPReachAlgorithm::run() {

    if(initialized){
        return;
    }

    partition();

    for(const auto &[_,algorithm] : graphToAlgorithmMap){
        algorithm -> run();

        (void)(_);  //unused warning
    }

    initialized = true;
}









PartitionedDAPReachAlgorithm::~PartitionedDAPReachAlgorithm() {
    deleteOldPartition();
}


void PartitionedDAPReachAlgorithm::deleteOldPartition() {
    for(auto [graph,algorithm] : graphToAlgorithmMap){
        delete algorithm;
        delete graph;
    }

    delete overlayGraph;

    mainToOverlayMap.resetAll();
    mainToSubMap.resetAll();
    graphToAlgorithmMap.resetAll();
    edgeVertices.resetAll();
}


void
PartitionedDAPReachAlgorithm::initAlgorithms(std::vector<Algora::DiGraph *> &graphs) {

    //create new Algorithms
    for (Algora::DiGraph *graph : graphs) {
        DynamicAPReachAlgorithm* algorithm = createSubAlgorithm();
        algorithm -> setGraph(graph);
        graphToAlgorithmMap.setValue(graph, algorithm);
    }
}


void PartitionedDAPReachAlgorithm::onVertexAdd(Algora::Vertex *vertex) {

    if(!initialized){
        return;
    }

    if(repartitionThreshold && ++operationsSinceRepartition >= repartitionThreshold){
        initialized = false;
    } else {

        Algora::DiGraph *subGraph = nullptr;

        //TODO chose smart or random graph
        for(auto anyVertex : mainToSubMap){
            if(anyVertex){
                subGraph = dynamic_cast<Algora::DiGraph*>(anyVertex->getParent());
                break;
            }
        }
        if(!subGraph){
            throw std::logic_error("Could not find any non empty graph to insert new Vertex");  //TODO something better (can't use graphToAlgorithm map because graphs are const inside
        }
        auto* subVertex = subGraph->addVertex();
        mainToSubMap[vertex] = subVertex;

    }

    DynamicDiGraphAlgorithm::onVertexAdd(vertex);
}


void PartitionedDAPReachAlgorithm::onVertexRemove(Algora::Vertex *vertex) {

    if(!initialized){
        return;
    }

    if(repartitionThreshold && ++operationsSinceRepartition >= repartitionThreshold){
        initialized = false;
    } else {

        Algora::Vertex *subVertex = mainToSubMap[vertex];
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
    DynamicDiGraphAlgorithm::onVertexRemove(vertex);
}


void PartitionedDAPReachAlgorithm::onArcAdd(Algora::Arc *arc) {

    if(!initialized){
        return;
    }

    DynamicDiGraphAlgorithm::onArcAdd(arc);

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
            subGraph->addArc(subTail, subHead);

            if ( k > 2) insertOverlayEdgeArcs(subGraph);
        } else {
            auto *overlayHead = mainToOverlayMap[mainHead];
            auto *overlayTail = mainToOverlayMap[mainTail];

            if (!overlayHead) {
                overlayHead = overlayGraph->addVertex();
                mainToOverlayMap[mainHead] = overlayHead;
                edgeVertices[headGraph].insert(mainHead);
                if ( k > 2) insertOverlayEdgeArcsOfVertex(mainHead);
            }
            if (!overlayTail) {
                overlayTail = overlayGraph->addVertex();
                mainToOverlayMap[mainTail] = overlayTail;
                edgeVertices[tailGraph].insert(mainTail);
                if ( k > 2) insertOverlayEdgeArcsOfVertex(mainTail);
            }

            overlayGraph->addArc(overlayTail, overlayHead);
        }
    }
}


void PartitionedDAPReachAlgorithm::onArcRemove(Algora::Arc *arc) {

    if(!initialized){
        return;
    }

    DynamicDiGraphAlgorithm::onArcRemove(arc);


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

            if( k > 2) {
                removeOverlayEdgeArcs(subGraph);
            }
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


void
PartitionedDAPReachAlgorithm::partition() {
    const Algora::FastPropertyMap<unsigned long long> &partitionMap = partitionFunction(k,diGraph);

    deleteOldPartition();

    std::vector<Algora::DiGraph*> subGraphs;

    for(auto i = 0ULL; i < k; i++){
        auto* graph = new Algora::IncidenceListGraph;

        graph->setParent(diGraph);

        subGraphs.push_back(graph);
    }
    overlayGraph = new Algora::IncidenceListGraph;
    overlayGraph->setParent(diGraph);



    diGraph->mapVertices([&subGraphs, this, partitionMap](Algora::Vertex* vertex){
        auto* subVertex = subGraphs.at(partitionMap.getValue(vertex))->addVertex();
        mainToSubMap[vertex] = subVertex;
    });

    diGraph->mapArcs([&subGraphs, this, partitionMap](Algora::Arc* arc){

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

    initAlgorithms(subGraphs);


    //add arcs that go through subgraphs
    if( k > 2) {
        for( Algora::DiGraph* diGraph : subGraphs){
            insertOverlayEdgeArcs(diGraph);
        }
    }

    operationsSinceRepartition = 0;
}


void PartitionedDAPReachAlgorithm::onDiGraphSet() {
    DynamicDiGraphAlgorithm::onDiGraphSet();

    initialized = false;
}


void PartitionedDAPReachAlgorithm::onDiGraphUnset() {
    DynamicDiGraphAlgorithm::onDiGraphUnset();
}


void PartitionedDAPReachAlgorithm::insertOverlayEdgeArcs(Algora::DiGraph *subGraph){
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
            if(newReachableEdges.count(arc->getHead())){
                newReachableEdges.erase(arc->getHead());
            }
        }, [&newReachableEdges](const Algora::Arc* arc){
            return newReachableEdges.empty();
        });

        for(Algora::Vertex* vertex: newReachableEdges){
            overlayGraph->addArc(overlaySource, vertex);
        }
    }
}


//It can't be that there is an Arc between two vertices of the same subgraph in the overlaygraph, but not connection in the subgraph
//, since tail and head are in the subgraph, their connection must also be in the subgraph!!!
void PartitionedDAPReachAlgorithm::removeOverlayEdgeArcs(Algora::DiGraph *subGraph) {
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

        std::unordered_set<Algora::Arc*> arcsToRemove{};
        overlayGraph->mapOutgoingArcsUntil(overlaySource, [&unreachableVertices, &arcsToRemove](Algora::Arc* arc){
            if(unreachableVertices.count(arc->getHead())){
                arcsToRemove.insert(arc);
            }
        },[&unreachableVertices, &arcsToRemove](const Algora::Arc* arc){
            return unreachableVertices.size() == arcsToRemove.size();
        });

        for(Algora::Arc* arc : arcsToRemove){
            overlayGraph->removeArc(arc);
        }
    }
}

bool PartitionedDAPReachAlgorithm::checkIfOverlayIsolated(Algora::Vertex *vertex) {

    bool overlayIsolated = true;

    auto* subGraph = mainToSubMap[vertex]->getParent();

    diGraph->mapOutgoingArcsUntil(vertex, [this, &overlayIsolated, &subGraph] (Algora::Arc* arc){
        overlayIsolated = mainToSubMap[arc->getHead()] == subGraph;
    }, [&overlayIsolated](const Algora::Arc* arc){
        return !overlayIsolated;
    });
    diGraph->mapIncomingArcsUntil(vertex, [this, &overlayIsolated, &subGraph] (Algora::Arc* arc){
        overlayIsolated = mainToSubMap[arc->getTail()] == subGraph;
    }, [&overlayIsolated](const Algora::Arc* arc){
        return !overlayIsolated;
    });

    return overlayIsolated;
}

void PartitionedDAPReachAlgorithm::removeOverlayVertex(Algora::Vertex *vertex) {

    auto* subGraph = mainToSubMap[vertex]->getParent();

    edgeVertices[subGraph].erase(vertex);
    overlayGraph->removeVertex(mainToOverlayMap[vertex]);
    mainToOverlayMap.resetToDefault(vertex);
}

void PartitionedDAPReachAlgorithm::insertOverlayEdgeArcsOfVertex(Algora::Vertex *vertex) {

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
