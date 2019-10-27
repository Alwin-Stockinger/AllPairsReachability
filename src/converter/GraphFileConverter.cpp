//
// Created by Alwin Stockinger.
//

#include <sstream>
#include <set>
#include <konectnetworkinstanceprovider.h>
#include <randominstanceprovider.h>
#include <property/fastpropertymap.h>
#include <cstring>
#include <spawn.h>
#include <wait.h>
#include "GraphFileConverter.h"




Algora::DynamicDiGraph * GraphFileConverter::readGraph(const std::string &fileName) {

    auto * graph = new Algora::DynamicDiGraph;


    //addVertices(fileName, graph); //TODO change or remove

    std::ifstream graphFile(fileName);
    if(graphFile.good()){
        std::string line;


        std::getline(graphFile, line); //skip firstline
        for(unsigned i = 1 ; std::getline(graphFile, line); i++){

            std::stringstream ss(line);
            std::string neighbourString;

            while(std::getline(ss, neighbourString, ' ')){
                graph->addArc(i , std::stoul(neighbourString), 0);
            }
        }
        graph->applyNextDelta();
        return graph;
    }
    else{
        //TODO error
        std::cerr << "GraphFile not good";
        return graph;
    }
}

Algora::DynamicDiGraph *
GraphFileConverter::makeOverlay(const Algora::DynamicDiGraph &mainGraph,
                                const std::map<unsigned long long, unsigned long long> &partitionMap) {

    auto * graph = new Algora::DynamicDiGraph;

    addVertices(mainGraph, graph);

    mainGraph.getDiGraph()->mapArcs([graph, partitionMap](Algora::Arc* arc){
        unsigned long long headId = arc->getHead()->getId();
        unsigned long long tailId = arc->getTail()->getId();
        if(partitionMap.at(headId) != partitionMap.at(tailId)){
            graph->addArc(tailId, headId,0);
        }
    });
    graph->applyNextDelta();
    return graph;
}

void GraphFileConverter::addVertices(const Algora::DynamicDiGraph &oldGraph, Algora::DynamicDiGraph *newGraph) {

    oldGraph.getDiGraph()->mapVertices([newGraph](Algora::Vertex* vertex){
       newGraph->addVertex(vertex->getId(), 0);
    });
}




std::vector<Algora::DynamicDiGraph *> *
GraphFileConverter::makeSubGraphs(const Algora::DynamicDiGraph& mainGraph, const std::map<unsigned long long, unsigned long long> &partitionMap) {

    auto* subGraphs = new std::vector<Algora::DynamicDiGraph*>();

    auto k = std::max_element(partitionMap.begin(), partitionMap.end(),[](const auto & v1,const auto &v2){
        return v1.second < v2.second;
    })->second + 1;

    for(auto i = 0; i < k; i++){
        subGraphs->push_back(new Algora::DynamicDiGraph);
    }

    addSubVertices(partitionMap, subGraphs);

    mainGraph.getDiGraph()->mapArcs([partitionMap, subGraphs](const Algora::Arc* arc){
        unsigned long long headId = arc->getHead()->getId();
        unsigned long long tailId = arc->getTail()->getId();
        if(partitionMap.at(headId) == partitionMap.at(tailId)){
            subGraphs->at(partitionMap.at(headId))->addArc(tailId, headId, 0);
        }
    });

    for(auto graph: *subGraphs){
        graph->applyNextDelta();
    }

    return subGraphs;
}

void
GraphFileConverter::addSubVertices(const std::map<unsigned long long, unsigned long long> &partitionMap,
                                   std::vector<Algora::DynamicDiGraph*> *graphs) {
    for(const auto &[id, partition]: partitionMap){
        graphs->at(partition)->addVertex(id, 0);
    }
}





