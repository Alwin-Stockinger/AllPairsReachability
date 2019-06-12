//
// Created by Alwin Stockinger.
//

#include "PartitionedDAPReachAlgorithmImplementation.h"

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
void PartitionedDAPReachAlgorithmImplementation<T>::run() {
    overlayAlgorithm -> run();
    for(const auto &[_,algorithm] : graphToAlgorithmMap){
        algorithm -> run();
    }

}

template<typename T>
std::string PartitionedDAPReachAlgorithmImplementation<T>::getName() const noexcept {
    return std::__cxx11::string("Partitioned Graph All Pair Reachability Algorithm based on " + overlayAlgorithm -> getName()+"");
}

template<typename T>
std::string PartitionedDAPReachAlgorithmImplementation<T>::getShortName() const noexcept {
    return std::__cxx11::string("Partitioned DAP Reach Algo(" + overlayAlgorithm -> getShortName()+")");
}

template<typename T>
bool PartitionedDAPReachAlgorithmImplementation<T>::query(const Algora::Vertex *start, const Algora::Vertex *end) {

    start = mainToOverlayMap[start];
    start = inMap[start];
    end = mainToOverlayMap[end];
    end = inMap[end];

    //select subgraph algorithms
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

        std::set<Algora::Vertex *>& startEdgeVertices = edgeVertices[start->getParent()];
        std::set<Algora::Vertex *> endEdgeVertices = edgeVertices[end->getParent()];    //must be copy, otherwise cant delete vertices from set

        //find outgoing vertices
        for (Algora::Vertex *outVertex : startEdgeVertices) {

            if (startGraphAlgorithm->query(start, inMap[outVertex])) {

                for(auto it = endEdgeVertices.begin(); it != endEdgeVertices.end();){//Algora::Vertex *inVertex : endEdgeVertices){

                    Algora::Vertex* inVertex = *it;

                    if( overlayAlgorithm->query(outVertex, inVertex)){

                        if( endGraphAlgorithm->query(inMap[inVertex], end)){
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


template<typename T>
PartitionedDAPReachAlgorithmImplementation<T>::~PartitionedDAPReachAlgorithmImplementation() {
    deleteOldPartition();
}

template<typename T>
void PartitionedDAPReachAlgorithmImplementation<T>::deleteOldPartition() {
    for(auto [graph,algorithm] : graphToAlgorithmMap){
        delete algorithm;
        delete graph;
    }

    delete overlayAlgorithm;
    delete overlayGraph;

    /*if(diGraph && mainToOverlayMap[diGraph->getAnyVertex()]){
        delete mainToOverlayMap[diGraph->getAnyVertex()]->getParent();  //why is there a memory leak when done this way???
    }*/

    mainToOverlayMap.resetAll();
    inMap.resetAll();
    graphToAlgorithmMap.resetAll();
    edgeVertices.resetAll();
}

template<typename T>
void
PartitionedDAPReachAlgorithmImplementation<T>::initAlgorithms(std::vector<Algora::DiGraph *> &graphs) {

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
void PartitionedDAPReachAlgorithmImplementation<T>::initEdges(const std::vector<Algora::DiGraph *> &graphs) {

    overlayGraph->mapVertices([this](Algora::Vertex* vertex){
        if(!overlayGraph->isIsolated(vertex)){
            Algora::Vertex* subVertex = inMap[vertex];
            edgeVertices[subVertex->getParent()].insert(vertex);
        }
    });
}

template<typename T>
void PartitionedDAPReachAlgorithmImplementation<T>::onVertexAdd(Algora::Vertex *vertex) {
    throw std::logic_error("Not implemented");
}

template<typename T>
void PartitionedDAPReachAlgorithmImplementation<T>::onVertexRemove(Algora::Vertex *vertex) {
    throw std::logic_error("Not implemented");
}

template<typename T>
void PartitionedDAPReachAlgorithmImplementation<T>::onArcAdd(Algora::Arc *arc) {
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
void PartitionedDAPReachAlgorithmImplementation<T>::onArcRemove(Algora::Arc *arc) {
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
PartitionedDAPReachAlgorithmImplementation<T>::partition(const Algora::FastPropertyMap<unsigned long long> &partitionMap,
                                           const unsigned long long k) {

    deleteOldPartition();

    std::vector<Algora::DiGraph*> subGraphs;

    for(auto i = 0ULL; i < k; i++){
        subGraphs.push_back(new Algora::IncidenceListGraph);
    }
    overlayGraph = new Algora::IncidenceListGraph;



    diGraph->mapVertices([&subGraphs, this, partitionMap](Algora::Vertex* vertex){
        Algora::Vertex* subVertex = subGraphs.at(partitionMap.getValue(vertex))->addVertex();
        Algora::Vertex* overlayVertex = overlayGraph->addVertex();

        mainToOverlayMap[vertex] = overlayVertex;
        inMap[overlayVertex] = subVertex;
    });

    diGraph->mapArcs([&subGraphs, this, partitionMap](Algora::Arc* arc){
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

    initAlgorithms(subGraphs);
    initEdges(subGraphs);

    partitioned = true;
}

template<typename T>
void PartitionedDAPReachAlgorithmImplementation<T>::onDiGraphSet() {
    partitioned = false;
    DynamicDiGraphAlgorithm::onDiGraphSet();
}

template<typename T>
void PartitionedDAPReachAlgorithmImplementation<T>::onDiGraphUnset() {
    partitioned = false;
    DynamicDiGraphAlgorithm::onDiGraphUnset();
}



template class PartitionedDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithm<Algora::StaticBFSSSReachAlgorithm>>;
template class PartitionedDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithm<Algora::StaticDFSSSReachAlgorithm>>;
template class PartitionedDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithm<Algora::LazyBFSSSReachAlgorithm>>;
template class PartitionedDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithm<Algora::LazyDFSSSReachAlgorithm>>;
template class PartitionedDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithm<Algora::CachingDFSSSReachAlgorithm>>;
template class PartitionedDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithm<Algora::CachingBFSSSReachAlgorithm>>;
template class PartitionedDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithm<Algora::SimpleIncSSReachAlgorithm>>;
template class PartitionedDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithm<Algora::ESTreeML>>;
template class PartitionedDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithm<Algora::OldESTree>>;
template class PartitionedDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithm<Algora::ESTreeQ>>;
template class PartitionedDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithm<Algora::SimpleESTree>>;