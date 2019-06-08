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

template<typename T>
void PartitionedDAPReachAlgorithm<T>::run() {
    overlayAlgorithm -> run();
    for(const auto &[_,algorithm] : graphToAlgorithmMap){
        algorithm -> run();
    }

}

template<typename T>
std::string PartitionedDAPReachAlgorithm<T>::getName() const noexcept {
    return std::__cxx11::string("Partitioned Graph All Pair Reachability Algorithm based on " + overlayAlgorithm -> getName()+"");
}

template<typename T>
std::string PartitionedDAPReachAlgorithm<T>::getShortName() const noexcept {
    return std::__cxx11::string("Partitioned DAP Reach Algo(" + overlayAlgorithm -> getShortName()+")");
}

template<typename T>
bool PartitionedDAPReachAlgorithm<T>::query(const Algora::Vertex *start, const Algora::Vertex *end) {

    start = mainToOverlayMap[start];
    start = inMap[start];
    end = mainToOverlayMap[end];
    end = inMap[end];

    //select subgraphalgorithms
    DynamicAPReachAlgorithm* startGraphAlgorithm = graphToAlgorithmMap[start->getParent()];
    DynamicAPReachAlgorithm* endGraphAlgorithm = graphToAlgorithmMap[end->getParent()];

    if(!startGraphAlgorithm || !endGraphAlgorithm){
        //TODO throw exception?
        return false;
    }

    //same subgraph? then normal Algorithm
    if(startGraphAlgorithm == endGraphAlgorithm){
        return startGraphAlgorithm -> query(start, end);
    }
    else {

        std::set<Algora::Vertex *> reachableEdges;

        std::set<Algora::Vertex *> startEdgeVertices = edgeVertices[start->getParent()];
        std::set<Algora::Vertex *> endEdgeVertices = edgeVertices[end->getParent()];
        std::set<Algora::Vertex *> overlayConnectedVertices;

        //find outgoing vertices
        for (Algora::Vertex *outVertex : startEdgeVertices) {

            if (startGraphAlgorithm->query(start, inMap[outVertex])) {


                for(Algora::Vertex *inVertex : endEdgeVertices){

                    if( overlayAlgorithm->query(outVertex, inVertex)){

                        if( endGraphAlgorithm->query(inMap[inVertex], end)){
                            return true;
                        }
                        else endEdgeVertices.erase(inVertex);    //exclude for future queries, because dead-end
                    }
                }
            }
        }

        //return that no connection was found
        return false;
    }

}


template<typename T>
PartitionedDAPReachAlgorithm<T>::~PartitionedDAPReachAlgorithm() {
    deleteOldPartition();
}

template<typename T>
void PartitionedDAPReachAlgorithm<T>::deleteOldPartition() {
    for(auto [graph,algorithm] : graphToAlgorithmMap){
        delete algorithm;
        delete graph;
    }
    delete overlayAlgorithm;
    delete mainToOverlayMap[diGraph->getAnyVertex()];

    mainToOverlayMap.resetAll();
    inMap.resetAll();
    graphToAlgorithmMap.resetAll();
    edgeVertices.resetAll();
}

template<typename T>
void
PartitionedDAPReachAlgorithm<T>::initAlgorithms(std::vector<Algora::DiGraph *> &graphs, Algora::DiGraph *overlayGraph) {



    //create new Algorithms
    overlayAlgorithm = new T();
    overlayAlgorithm->setGraph(overlayGraph);
    for (Algora::DiGraph *graph : graphs) {
        T* algorithm = new T();
        algorithm -> setGraph(graph);
        graphToAlgorithmMap.setValue(graph, algorithm);

    }

}

template<typename T>
void PartitionedDAPReachAlgorithm<T>::initEdges(const std::vector<Algora::DiGraph*> &graphs, Algora::DiGraph *overlayGraph){

    overlayGraph->mapVertices([overlayGraph, this](Algora::Vertex* vertex){
        if(!overlayGraph->isIsolated(vertex)){
            Algora::Vertex* subVertex = inMap[vertex];
            edgeVertices[subVertex->getParent()].insert(vertex);
        }
    });
}

template<typename T>
void PartitionedDAPReachAlgorithm<T>::onVertexAdd(Algora::Vertex *vertex) {

    throw "Not implemented";
    DynamicDiGraphAlgorithm::onVertexAdd(vertex);
}

template<typename T>
void PartitionedDAPReachAlgorithm<T>::onVertexRemove(Algora::Vertex *vertex) {

    throw "Not implemented";

    DynamicDiGraphAlgorithm::onVertexRemove(vertex);
}

template<typename T>
void PartitionedDAPReachAlgorithm<T>::onArcAdd(Algora::Arc *arc) {
    DynamicDiGraphAlgorithm::onArcAdd(arc);

    auto * overlayHead = mainToOverlayMap[arc->getHead()];
    auto * overlayTail = mainToOverlayMap[arc->getTail()];

    auto * subHead = inMap[overlayHead];
    auto * subTail = inMap[overlayTail];


    auto* headGraph = subHead->getParent();
    auto* tailGraph = subTail->getParent();


    //auto* headAlgorithm = findAlgorithm(inMap[overlayHead]);
    //auto* tailAlgorithm = findAlgorithm(inMap[overlayTail]);


    if(headGraph == tailGraph){
        static_cast<Algora::DiGraph*>(headGraph)->addArc(subTail, subHead);
    }
    else{

        static_cast<Algora::DiGraph*>(overlayHead->getParent())->addArc(overlayTail, overlayHead);

        edgeVertices[headGraph].insert(overlayHead);
        edgeVertices[tailGraph].insert(overlayTail);
    }
}

template<typename T>
void PartitionedDAPReachAlgorithm<T>::onArcRemove(Algora::Arc *arc) {
    DynamicDiGraphAlgorithm::onArcRemove(arc);

    auto * overlayHead = mainToOverlayMap[arc->getHead()];
    auto * overlayTail = mainToOverlayMap[arc->getTail()];

    auto * subHead = inMap[overlayHead];
    auto * subTail = inMap[overlayTail];


    auto* headGraph = subHead->getParent();
    auto* tailGraph = subTail->getParent();

    if(headGraph == tailGraph){
        auto* subGraph = static_cast<Algora::DiGraph*>(headGraph);
        Algora::Arc* subArc = subGraph->findArc(subTail, subHead);
        subGraph->removeArc(subArc);
    }
    else{
        auto* overlayGraph = static_cast<Algora::DiGraph*>(overlayHead->getParent());
        Algora::Arc* overlayArc = overlayGraph->findArc(overlayTail, overlayHead);
        overlayGraph->removeArc(overlayArc);

        if(overlayGraph->isIsolated(overlayHead)){
            edgeVertices[headGraph].erase(overlayHead);
        }
        if(overlayGraph->isIsolated(overlayTail)){
            edgeVertices[tailGraph].erase(overlayTail);
        }
    }
}

template<typename T>
void
PartitionedDAPReachAlgorithm<T>::partition(const Algora::FastPropertyMap<unsigned long long> &partitionMap,
                                           const unsigned long long k) {

    deleteOldPartition();

    std::vector<Algora::DiGraph*> subGraphs;

    for(auto i = 0ULL; i < k; i++){
        subGraphs.push_back(new Algora::IncidenceListGraph);
    }
    Algora::DiGraph* overlayGraph = new Algora::IncidenceListGraph;



    diGraph->mapVertices([&subGraphs, &overlayGraph, this, partitionMap](Algora::Vertex* vertex){
        Algora::Vertex* subVertex = subGraphs.at(partitionMap.getValue(vertex))->addVertex();
        Algora::Vertex* overlayVertex = overlayGraph->addVertex();

        mainToOverlayMap[vertex] = overlayVertex;
        inMap[overlayVertex] = subVertex;
    });

    diGraph->mapArcs([&subGraphs, &overlayGraph, this, partitionMap](Algora::Arc* arc){
        unsigned long long headPartition = partitionMap.getValue(arc->getHead());
        unsigned long long tailPartition = partitionMap.getValue(arc->getTail());

        if( headPartition == tailPartition){
            Algora::Vertex* subTail = inMap.getValue(mainToOverlayMap.getValue(arc->getTail()));
            Algora::Vertex* subHead = inMap.getValue(mainToOverlayMap.getValue(arc->getHead()));
            subGraphs.at(headPartition)->addArc(subTail, subHead);
        }
        else{
            Algora::Vertex* overlayTail = mainToOverlayMap.getValue(arc->getTail());
            Algora::Vertex* overlayHead = mainToOverlayMap.getValue(arc->getHead());

            overlayGraph->addArc(overlayTail, overlayHead);
        }

    });

    initAlgorithms(subGraphs, overlayGraph);
    initEdges(subGraphs, overlayGraph);
}

template class PartitionedDAPReachAlgorithm<SSBasedDAPReachAlgorithm<Algora::StaticBFSSSReachAlgorithm>>;
template class PartitionedDAPReachAlgorithm<SSBasedDAPReachAlgorithm<Algora::StaticDFSSSReachAlgorithm>>;
template class PartitionedDAPReachAlgorithm<SSBasedDAPReachAlgorithm<Algora::LazyBFSSSReachAlgorithm>>;
template class PartitionedDAPReachAlgorithm<SSBasedDAPReachAlgorithm<Algora::LazyDFSSSReachAlgorithm>>;
template class PartitionedDAPReachAlgorithm<SSBasedDAPReachAlgorithm<Algora::CachingDFSSSReachAlgorithm>>;
template class PartitionedDAPReachAlgorithm<SSBasedDAPReachAlgorithm<Algora::CachingBFSSSReachAlgorithm>>;
template class PartitionedDAPReachAlgorithm<SSBasedDAPReachAlgorithm<Algora::SimpleIncSSReachAlgorithm>>;
template class PartitionedDAPReachAlgorithm<SSBasedDAPReachAlgorithm<Algora::ESTreeML>>;
template class PartitionedDAPReachAlgorithm<SSBasedDAPReachAlgorithm<Algora::OldESTree>>;
template class PartitionedDAPReachAlgorithm<SSBasedDAPReachAlgorithm<Algora::ESTreeQ>>;
template class PartitionedDAPReachAlgorithm<SSBasedDAPReachAlgorithm<Algora::SimpleESTree>>;