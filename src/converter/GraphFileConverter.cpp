//
// Created by Alwin Stockinger.
//

#include <sstream>
#include <set>
#include <konectnetworkinstanceprovider.h>
#include <randominstanceprovider.h>
#include "GraphFileConverter.h"


//TODO make ids unsigned long long



void GraphFileConverter::convertDiGraphToKahip(Algora::DiGraph *graph, const std::string& outputFileName){
    std::map<unsigned long long int, std::map<unsigned long long int, unsigned long long int>> vertexMap = generateVertexMapFromGraph(graph);
    writeMapToFile(outputFileName, graph, vertexMap);
}

void GraphFileConverter::writeMapToFile(const std::string &outFileName, Algora::DiGraph *graph,
                                        std::map<unsigned long long, std::map<unsigned long long, unsigned long long>> vertexMap) {


    std::ofstream outFile(outFileName, std::ios::trunc);

    unsigned long long arcSize = 0;

    for(const auto& [_,map]: vertexMap){
        arcSize += map.size();
    }
    arcSize /= 2;

    if(outFile.good() && outFile.is_open()){
        outFile << graph->getSize() << " " << arcSize << " 1\n";
        outFile.flush();
    }

    for(unsigned long long i = 0; i < graph->getSize(); i++){
        if(vertexMap.find(i) != vertexMap.end()){
            for(const auto &[tail, weight]: vertexMap[i]){
                outFile << tail + 1 << " " << weight << " ";
            }
        }
        outFile << "\n";
        outFile.flush();
    }

    outFile.close();
}

std::map<unsigned long long int, std::map<unsigned long long int, unsigned long long int>> GraphFileConverter::generateVertexMapFromGraph(
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


void GraphFileConverter::convertKahipDiToUnDi(const std::string &inName, const std::string &outName) {
    std::ifstream inFile(inName);
    std::ofstream outFile(outName, std::ios::trunc);

    //TODO Error check

    std::string line;
    std::getline(inFile, line);


    if(outFile.good() && outFile.is_open()){
        outFile << line <<" 1\n";

        outFile.flush();
    }


    std::map<unsigned , std::set<unsigned >> vertexMap;

    for(unsigned i = 1; std::getline(inFile, line); i++){
        //TODO Fix reading last empty line

        std::string arcString;
        std::stringstream ss(line);

        if(line.empty()){
            vertexMap[i] = std::set<unsigned>();
        }
        else while(std::getline(ss, arcString,' ')){
            unsigned neighbour = std::stoul(arcString);
            vertexMap[i].insert(neighbour);
        }
    }

    for( const auto &[id, set] : vertexMap){
        for( const auto &[neighbourId, neighbourSet] : vertexMap){
            if(set.find(neighbourId) != set.end() && neighbourSet.find(id) != neighbourSet.end()){
                outFile << neighbourId << " 2 ";
            }
            else if(set.find(neighbourId) != set.end() || neighbourSet.find(id) != neighbourSet.end()){
                outFile << neighbourId << " 1 ";
            }
        }
        outFile << '\n';
        outFile.flush();
    }


    outFile.close();
}



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

std::map<unsigned long long, unsigned long long> GraphFileConverter::makePartitionMap(const std::string &partitionFileName) {

    std::map<unsigned long long, unsigned long long> partitionMap;

    std::ifstream file(partitionFileName);

    if(file.good()) {
        std::string line;
        for (unsigned i = 0; std::getline(file, line); i++) {
            partitionMap[i] = std::stoul(line);
        }
    }
    else{
        //TODO error
        std::cerr << "PartitionFile not good";
    }

    return partitionMap;
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

std::map<const Algora::Vertex *, Algora::Vertex *> *
GraphFileConverter::makeMainToOverlayMap(const Algora::DynamicDiGraph& mainGraph,const Algora::DynamicDiGraph *overlayGraph) {

    auto* vertexMap = new std::map<const Algora::Vertex*, Algora::Vertex*>;

    for(unsigned i = 0; i < mainGraph.getCurrentGraphSize(); i++){
        Algora::Vertex* keyVertex = mainGraph.getCurrentVertexForId(i);
        vertexMap->insert({keyVertex, overlayGraph->getCurrentVertexForId(i)});
    }
    return vertexMap;
}

std::map<const Algora::Vertex *, Algora::Vertex *> *
GraphFileConverter::makeInMap(Algora::DynamicDiGraph *overlayGraph, std::vector<Algora::DynamicDiGraph *> *subGraphs,
                              std::map<unsigned long long int, unsigned long long int> partitionMap) {
    auto* vertexMap = new std::map<const Algora::Vertex*, Algora::Vertex*>();

    for(unsigned i = 0; i < overlayGraph->getCurrentGraphSize(); i++){
        Algora::DynamicDiGraph* subGraph = subGraphs->at(partitionMap[i]);
        Algora::Vertex* subVertex = subGraph->getCurrentVertexForId(i);
        vertexMap->insert({overlayGraph->getCurrentVertexForId(i),subVertex});
    }
    return vertexMap;
}






