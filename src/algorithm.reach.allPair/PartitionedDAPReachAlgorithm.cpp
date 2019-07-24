//
// Created by Alwin Stockinger.
//

#include "PartitionedDAPReachAlgorithm.h"

#include <algorithm.reach/staticbfsssreachalgorithm.h>
#include <algorithm.reach/staticdfsssreachalgorithm.h>
#include <algorithm.reach/cachingbfsssreachalgorithm.h>
#include <algorithm.reach/cachingdfsssreachalgorithm.h>
#include <algorithm.reach/lazydfsssreachalgorithm.h>
#include <algorithm.reach/lazybfsssreachalgorithm.h>
#include <algorithm.reach/simpleincssreachalgorithm.h>
#include <algorithm.reach.es/estree-ml.h>
#include <algorithm.reach.es/estree-bqueue.h>
#include <algorithm.reach.es/estree-queue.h>
#include <algorithm.reach.es/simpleestree.h>

#include <graph.incidencelist/incidencelistgraph.h>

void PartitionedDAPReachAlgorithm::run() {

    if(initialized){
        return;
    }

    partition();

    overlayAlgorithm -> run();
    for(const auto &[_,algorithm] : graphToAlgorithmMap){
        algorithm -> run();

        (void)(_);  //unused warning
    }

    initialized = true;
}


std::string PartitionedDAPReachAlgorithm::getName() const noexcept {
    return std::__cxx11::string("Partitioned Graph All Pair Reachability Algorithm based on " + overlayAlgorithm -> getName()+"");
}


std::string PartitionedDAPReachAlgorithm::getShortName() const noexcept {
    return std::__cxx11::string("Partitioned DAP Reach Algo(" + overlayAlgorithm -> getShortName()+")");
}


bool PartitionedDAPReachAlgorithm::query(Algora::Vertex *start, const Algora::Vertex *end) {
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
        DynamicAPReachAlgorithm* startGraphAlgorithm = graphToAlgorithmMap[startGraph];
        DynamicAPReachAlgorithm* endGraphAlgorithm = graphToAlgorithmMap[endGraph];

        std::set<Algora::Vertex *>& startEdgeVertices = edgeVertices[startGraph];
        std::set<Algora::Vertex *> endEdgeVertices = edgeVertices[endGraph];    //must be copy, otherwise can't erase vertices from set

        //find outgoing vertices
        for (Algora::Vertex *outVertex : startEdgeVertices) {

            /*if(endEdgeVertices.empty()){ very unlikely to improve algorithm, because there would have to be a connection between every startEdgeVertex and endEdgeVertex, but the connections should be minimized by partitioning
                //there is not a single endEdgeVertex that has a connection to the end vertex
                return false;
            }*/

            if (startGraphAlgorithm->query(start, mainToSubMap[outVertex])) {

                for(auto it = endEdgeVertices.begin(); it != endEdgeVertices.end();){

                    Algora::Vertex* inVertex = *it;

                    if( overlayAlgorithm->query(mainToOverlayMap[outVertex], mainToOverlayMap[inVertex])){

                        if( endGraphAlgorithm->query(mainToSubMap[inVertex], end)){
                            return true;
                        }
                        else{
                            it = endEdgeVertices.erase(it);    //exclude for future queries, because dead-end
                        }
                    }
                    else it++;
                }
            }
        }

        //return that no connection was found
        return false;
    }

}



PartitionedDAPReachAlgorithm::~PartitionedDAPReachAlgorithm() {
    deleteOldPartition();
}


void PartitionedDAPReachAlgorithm::deleteOldPartition() {
    for(auto [graph,algorithm] : graphToAlgorithmMap){
        delete algorithm;
        delete graph;
    }

    delete overlayAlgorithm;
    delete overlayGraph;

    mainToOverlayMap.resetAll();
    mainToSubMap.resetAll();
    graphToAlgorithmMap.resetAll();
    edgeVertices.resetAll();
}


void
PartitionedDAPReachAlgorithm::initAlgorithms(std::vector<Algora::DiGraph *> &graphs) {

    //create new Algorithms
    overlayAlgorithm = createOverlayAlgorithm();
    overlayAlgorithm->setGraph(overlayGraph);
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

    if(++operationsSinceRepartition > repartitionThreshold){
        partition();
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

    if(++operationsSinceRepartition > repartitionThreshold){
        partition();
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

    if(++operationsSinceRepartition > repartitionThreshold){
        partition();
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

            insertOverlayEdgeArcs(subGraph);
        } else {
            auto *overlayHead = mainToOverlayMap[mainHead];
            auto *overlayTail = mainToOverlayMap[mainTail];

            if (!overlayHead) {
                overlayHead = overlayGraph->addVertex();
                mainToOverlayMap[mainHead] = overlayHead;
                edgeVertices[headGraph].insert(mainHead);
            }
            if (!overlayTail) {
                overlayTail = overlayGraph->addVertex();
                mainToOverlayMap[mainTail] = overlayTail;
                edgeVertices[tailGraph].insert(mainTail);
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


    if(++operationsSinceRepartition > repartitionThreshold){
        partition();
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

            if (overlayGraph->isIsolated(overlayHead)) {
                mainToOverlayMap.resetToDefault(mainHead);
                edgeVertices[headGraph].erase(mainHead);
                overlayGraph->removeVertex(overlayHead);
            }
            if (overlayGraph->isIsolated(overlayTail)) {
                mainToOverlayMap.resetToDefault(mainTail);
                edgeVertices[tailGraph].erase(mainTail);
                overlayGraph->removeVertex(overlayTail);
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
    for( Algora::DiGraph* diGraph : subGraphs){
        insertOverlayEdgeArcs(diGraph);
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
    std::set<Algora::Vertex*>& edges = edgeVertices[subGraph];

    for(Algora::Vertex* source : edges){
        for(Algora::Vertex* destination : edges){
            if( source != destination){

                DynamicAPReachAlgorithm* subAlgorithm = graphToAlgorithmMap[subGraph];

                Algora::Vertex* subSource = mainToSubMap[source];
                Algora::Vertex* subDestination = mainToSubMap[destination];

                //vertices are already in overlay because they are edge vertices!
                Algora::Vertex* overlaySource = mainToOverlayMap[source];
                Algora::Vertex* overlayDestination = mainToOverlayMap[destination];

                if(overlayGraph->findArc(overlaySource, overlayDestination)){
                    continue;
                }
                else if(subAlgorithm->query(subSource, subDestination)){

                    overlayGraph->addArc(overlaySource, overlayDestination);
                }
            }
        }
    }
}


//It can't be that there is an Arc between two vertices of the same subgraph in the overlaygraph, but not in the subgraph
//, since tail and head are in the subgraph, their arc must also be in the subgraph!!!
void PartitionedDAPReachAlgorithm::removeOverlayEdgeArcs(Algora::DiGraph *subGraph) {
    std::set<Algora::Vertex*>& edges = edgeVertices[subGraph];

    for(Algora::Vertex* source : edges){
        for(Algora::Vertex* destination : edges){
            if( source != destination){

                DynamicAPReachAlgorithm* subAlgorithm = graphToAlgorithmMap[subGraph];

                Algora::Vertex* subSource = mainToSubMap[source];
                Algora::Vertex* subDestination = mainToSubMap[destination];

                //vertices are already in overlay because they are edge vertices!
                Algora::Vertex* overlaySource = mainToOverlayMap[source];
                Algora::Vertex* overlayDestination = mainToOverlayMap[destination];

                Algora::Arc* overlayArc = overlayGraph->findArc(overlaySource, overlayDestination);

                if(overlayArc && !subAlgorithm->query(subSource, subDestination)){
                    overlayGraph->removeArc(overlayArc);
                }
            }
        }
    }
}
