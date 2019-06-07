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

template<typename T>
void PartitionedDAPReachAlgorithm<T>::run() {
    overlayAlgorithm -> run();
    for(auto [_,algorithm] : graphToAlgorithmMap){
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
PartitionedDAPReachAlgorithm<T>::PartitionedDAPReachAlgorithm(std::vector<Algora::DiGraph *> &graphs,
                                                              Algora::DiGraph *overlayGraph,
                                                              Algora::FastPropertyMap< Algora::Vertex*>& inMap, Algora::FastPropertyMap< Algora::Vertex*>& mainToOverlayMap) {
    setGraphs(&graphs, overlayGraph, inMap, mainToOverlayMap);
}

template<typename T>
PartitionedDAPReachAlgorithm<T>::~PartitionedDAPReachAlgorithm() {
    deleteAlgorithms();
}

template<typename T>
void PartitionedDAPReachAlgorithm<T>::deleteAlgorithms() {
    for(auto [_,algorithm] : graphToAlgorithmMap){
        delete algorithm;
    }
    delete overlayAlgorithm;

    graphToAlgorithmMap.resetAll();
    edgeVertices.resetAll();
}

template<typename T>
void
PartitionedDAPReachAlgorithm<T>::setGraphs(std::vector<Algora::DiGraph *>* graphs, Algora::DiGraph *overlayGraph, Algora::FastPropertyMap< Algora::Vertex*>& inMap, Algora::FastPropertyMap< Algora::Vertex*>& mainToOverlayMap) {
    //delete old Algorithms
    deleteAlgorithms();

    this->inMap=inMap;
    this->mainToOverlayMap=mainToOverlayMap;

    //this->diGraph = overlayGraph; ??? will be overwritten anyway???

    //create new Algorithms
    overlayAlgorithm = new T();
    overlayAlgorithm->setGraph(overlayGraph);
    for (Algora::DiGraph *graph : *graphs) {
        //algorithm for graph setup
        T* algorithm = new T();
        algorithm -> setGraph(graph);
        graphToAlgorithmMap.setValue(graph, algorithm);

        //edges of graph setup
        std::set<Algora::Vertex*> edgesOfGraph;
        overlayGraph->mapVertices([&overlayGraph, &edgesOfGraph, graph, &inMap](Algora::Vertex *vertex) {
            if(!overlayGraph->isIsolated(vertex) && graph->containsVertex(inMap[vertex])){
                edgesOfGraph.insert(vertex);
            }
        });
        edgeVertices.setValue(graph, edgesOfGraph);
    }

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