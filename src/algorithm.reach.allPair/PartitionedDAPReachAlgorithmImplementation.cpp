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
    mainToSubMap.resetAll();
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
void PartitionedDAPReachAlgorithmImplementation<T>::onVertexAdd(Algora::Vertex *vertex) {
    Algora::DiGraph* subGraph = nullptr;

    //TODO chose smart or random graph
    for(auto it = mainToSubMap.begin(); it != mainToSubMap.end(); it++){
        auto* anyVertex = (*it);
        if(anyVertex){
            subGraph = static_cast<Algora::DiGraph*>(anyVertex->getParent());
            break;
        }
    }
    if(!subGraph){
        throw std::logic_error("Could not find any non empty graph to insert new Vertex");  //TODO something better (can't use graphToAlgorithm map because graphs are const inside
    }
    auto* subVertex = subGraph->addVertex();
    mainToSubMap[vertex] = subVertex;

    DynamicDiGraphAlgorithm::onVertexAdd(vertex);
}

template<typename T>
void PartitionedDAPReachAlgorithmImplementation<T>::onVertexRemove(Algora::Vertex *vertex) {
    Algora::Vertex* subVertex = mainToSubMap[vertex];
    Algora::Vertex* overlayVertex = mainToOverlayMap[vertex];

    auto * subGraph = dynamic_cast<Algora::DiGraph*>(subVertex->getParent());
    subGraph->removeVertex(subVertex);
    mainToSubMap.resetToDefault(vertex);

    if(overlayVertex){
        overlayGraph->removeVertex(overlayVertex);
        mainToOverlayMap.resetToDefault(vertex);
        edgeVertices[subGraph].erase(vertex);
    }
    DynamicDiGraphAlgorithm::onVertexRemove(vertex);
}

template<typename T>
void PartitionedDAPReachAlgorithmImplementation<T>::onArcAdd(Algora::Arc *arc) {
    DynamicDiGraphAlgorithm::onArcAdd(arc);

    auto* mainHead = arc->getHead();
    auto* mainTail = arc->getTail();

    auto * subHead = mainToSubMap[mainHead];
    auto * subTail = mainToSubMap[mainTail];

    auto* headGraph = subHead->getParent();
    auto* tailGraph = subTail->getParent();

    if(headGraph == tailGraph){
        static_cast<Algora::DiGraph*>(headGraph)->addArc(subTail, subHead);
    }
    else{
        auto * overlayHead = mainToOverlayMap[mainHead];
        auto * overlayTail = mainToOverlayMap[mainTail];

        if(!overlayHead){
            overlayHead = overlayGraph->addVertex();
            mainToOverlayMap[mainHead] = overlayHead;
            edgeVertices[headGraph].insert(mainHead);
        }
        if(!overlayTail){
            overlayTail = overlayGraph->addVertex();
            mainToOverlayMap[mainTail] = overlayTail;
            edgeVertices[tailGraph].insert(mainTail);
        }
        overlayGraph->addArc(overlayTail, overlayHead);
    }
}

template<typename T>
void PartitionedDAPReachAlgorithmImplementation<T>::onArcRemove(Algora::Arc *arc) {
    DynamicDiGraphAlgorithm::onArcRemove(arc);

    auto * mainHead = arc->getHead();
    auto * mainTail = arc->getTail();

    auto * subHead = mainToSubMap[mainHead];
    auto * subTail = mainToSubMap[mainTail];

    auto* headGraph = subHead->getParent();
    auto* tailGraph = subTail->getParent();

    if(headGraph == tailGraph){
        auto* subGraph = static_cast<Algora::DiGraph*>(headGraph);
        Algora::Arc* subArc = subGraph->findArc(subTail, subHead);
        subGraph->removeArc(subArc);
    }
    else{
        auto * overlayHead = mainToOverlayMap[mainHead];
        auto * overlayTail = mainToOverlayMap[mainTail];

        Algora::Arc* overlayArc = overlayGraph->findArc(overlayTail, overlayHead);
        overlayGraph->removeArc(overlayArc);

        if(overlayGraph->isIsolated(overlayHead)){
            mainToOverlayMap.resetToDefault(mainHead);
            edgeVertices[headGraph].erase(mainHead);
            overlayGraph->removeVertex(overlayHead);
        }
        if(overlayGraph->isIsolated(overlayTail)){
            mainToOverlayMap.resetToDefault(mainTail);
            edgeVertices[tailGraph].erase(mainTail);
            overlayGraph->removeVertex(overlayTail);
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