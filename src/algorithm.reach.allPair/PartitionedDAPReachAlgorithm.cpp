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
    for(T* algorithm : apAlgorithms){
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
    DynamicAPReachAlgorithm* startGraphAlgorithm = findAlgorithm(start);
    DynamicAPReachAlgorithm* endGraphAlgorithm = findAlgorithm(end);

    if(!startGraphAlgorithm || !endGraphAlgorithm){
        //TODO throw exception?
        return false;
    }

    //same subgraph? then normal Algorithm
    if(startGraphAlgorithm == endGraphAlgorithm){
        return startGraphAlgorithm -> query(start, end);
    }
    else {

        std::vector<Algora::Vertex *> reachableEdges;

        std::set<Algora::Vertex *> startEdgeVertices = edgeVertices[startGraphAlgorithm];
        std::set<Algora::Vertex *> endEdgeVertices = edgeVertices[endGraphAlgorithm];
        std::vector<Algora::Vertex *> overlayConnectedVertices;

        //find outgoing vertices
        for (Algora::Vertex *outVertex : startEdgeVertices) {

            if (startGraphAlgorithm->query(start, inMap[outVertex])) {


                for(Algora::Vertex *inVertex : endEdgeVertices){
                    if( overlayAlgorithm->query(outVertex, inVertex)){      //save which ends already visited?

                        if( endGraphAlgorithm->query(inMap[inVertex], end)){
                            return true;
                        }

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
                                                              std::map<const Algora::Vertex *, Algora::Vertex *> &inMap,
                                                              std::map<const Algora::Vertex *, Algora::Vertex *> &mainToOverlayMap) {
    setGraphs(&graphs, overlayGraph, inMap, mainToOverlayMap);
}

template<typename T>
PartitionedDAPReachAlgorithm<T>::~PartitionedDAPReachAlgorithm() {
    deleteAlgorithms();
}

template<typename T>
void PartitionedDAPReachAlgorithm<T>::deleteAlgorithms() {
    for(DynamicAPReachAlgorithm* algorithm: apAlgorithms){
        delete algorithm;
    }

    delete overlayAlgorithm;

    apAlgorithms.clear();

    edgeVertices.clear();
}

template<typename T>
void
PartitionedDAPReachAlgorithm<T>::setGraphs(std::vector<Algora::DiGraph *>* graphs, Algora::DiGraph *overlayGraph, std::map<const Algora::Vertex*, Algora::Vertex*>& inMap, std::map<const Algora::Vertex*, Algora::Vertex*>& mainToOverlayMap) {
    //delete old Algorithms
    deleteAlgorithms();

    this->inMap=inMap;
    this->mainToOverlayMap=mainToOverlayMap;

    this->diGraph = overlayGraph;

    //create new Algorithms
    overlayAlgorithm = new T();
    overlayAlgorithm->setGraph(overlayGraph);
    for (Algora::DiGraph *graph : *graphs) {
        T* algorithm = new T();
        algorithm -> setGraph(graph);
        apAlgorithms.insert( algorithm);

        std::set<Algora::Vertex*> edgesOfGraph;

        overlayGraph->mapVertices([&overlayGraph, &edgesOfGraph, graph, &inMap](Algora::Vertex *vertex) {
            if(!overlayGraph->isIsolated(vertex) && graph->containsVertex(inMap[vertex])){
                edgesOfGraph.insert(vertex);
            }
        });
        edgeVertices.insert({algorithm, edgesOfGraph});
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

    Algora::Vertex* overlayHead = mainToOverlayMap[arc->getHead()];
    Algora::Vertex* overlayTail = mainToOverlayMap[arc->getTail()];

    auto* headAlgorithm = findAlgorithm(inMap[overlayHead]);
    auto* tailAlgorithm = findAlgorithm(inMap[overlayTail]);


    if(headAlgorithm && (headAlgorithm == tailAlgorithm)){
        headAlgorithm->addArc(inMap[overlayHead], inMap[overlayTail]);
    }
    else{
        edgeVertices[headAlgorithm].insert(overlayHead);
        edgeVertices[tailAlgorithm].insert(overlayTail);
        overlayAlgorithm->addArc(overlayHead, overlayTail);
    }
}

template<typename T>
void PartitionedDAPReachAlgorithm<T>::onArcRemove(Algora::Arc *arc) {
    DynamicDiGraphAlgorithm::onArcRemove(arc);

    Algora::Vertex* overlayHead = mainToOverlayMap[arc->getHead()];
    Algora::Vertex* overlayTail = mainToOverlayMap[arc->getTail()];

    auto* headAlgorithm = findAlgorithm(inMap[overlayHead]);
    auto * tailAlgorithm = findAlgorithm(inMap[overlayTail]);

    if(headAlgorithm == tailAlgorithm){
        headAlgorithm->removeArc(inMap[overlayHead], inMap[overlayTail]);
    }
    else{
        overlayAlgorithm->removeArc(overlayHead, overlayTail);
        if(overlayAlgorithm->isIsolated(overlayHead)){
            edgeVertices[headAlgorithm].erase(overlayHead);
        }
        if(overlayAlgorithm->isIsolated(overlayTail)){
            edgeVertices[tailAlgorithm].erase(overlayTail);
        }
    }
}

template<typename T>
T *PartitionedDAPReachAlgorithm<T>::findAlgorithm(const Algora::Vertex *vertex) {
    for( T* apAlgorithm: apAlgorithms){
        if(apAlgorithm->containsVertex(vertex)){
            return apAlgorithm;
        }
    }
    return nullptr;
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