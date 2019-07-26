//
// Created by Alwin Stockinger.
//

#include <map>

#include "KahipAdapter.h"

#include "kaHIP_interface.h"

Algora::FastPropertyMap<unsigned long long>
KahipAdapter::handlePartitioning(unsigned long long int k, Algora::DiGraph *graph) {

    std::map<unsigned long long int, std::map<unsigned long long int, unsigned long long int>> arcMap =
            generateVertexMapFromGraph(graph);



    //kaffpa();
}


std::map<unsigned long long int, std::map<unsigned long long int, unsigned long long int>> KahipAdapter::generateVertexMapFromGraph(
        Algora::DiGraph *graph) {


    std::map<unsigned long long , std::map<unsigned long long, unsigned long long>> vertexMap;

    graph->mapArcs([&vertexMap](Algora::Arc* arc){
        if(!arc->isLoop()){
            unsigned long long headId = arc->getHead()->getId();
            unsigned long long tailId = arc->getTail()->getId();

            if(vertexMap.find(headId) == vertexMap.end()){
                vertexMap[headId][tailId] = 1;
                vertexMap[tailId][headId] = 1;
            } else{
                vertexMap[headId][tailId]++;
                vertexMap[tailId][headId]++;
            }
        }
    });

    return vertexMap;
}