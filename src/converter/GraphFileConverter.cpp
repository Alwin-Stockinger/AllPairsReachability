//
// Created by Alwin Stockinger.
//

#include <sstream>
#include <set>
#include "GraphFileConverter.h"




void GraphFileConverter::convertDiToUnDi(const std::string &inName, const std::string &outName) {
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


    addVertices(fileName, graph);

    std::ifstream graphFile(fileName);
    if(graphFile.good()){
        std::string line;


        std::getline(graphFile, line); //skip firstline
        for(unsigned i = 1 ; std::getline(graphFile, line); i++){

            std::stringstream ss(line);
            std::string neighbourString;

            while(std::getline(ss, neighbourString, ' ')){
                graph->addArc(std::stoul(neighbourString), i, 0);
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
GraphFileConverter::makeOverlay(const std::string &graphFileName, std::map<unsigned int, unsigned int> &partitionMap) {

    auto * graph = new Algora::DynamicDiGraph;

    addVertices(graphFileName, graph);

    std::ifstream graphFile(graphFileName);
    if(graphFile.good()){

        std::string graphLine;

        std::getline(graphFile, graphLine); //skip first line
        for(unsigned i = 1 ; std::getline(graphFile, graphLine); i++){

            std::stringstream graphSS(graphLine);

            std::string neighbourString;

            while(std::getline(graphSS, neighbourString, ' ')){
                unsigned neighbourId = std::stoul(neighbourString);

                if(partitionMap[i] != partitionMap[neighbourId]){
                    graph->addArc(neighbourId, i, 0);
                }
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

void GraphFileConverter::addVertices(const std::string &graphFileName, Algora::DynamicDiGraph *graph) {

    std::ifstream graphFile(graphFileName);

    if(graphFile.good()) {
        std::string line;
        std::getline(graphFile, line); //skip first line
        for (unsigned i = 1; std::getline(graphFile, line); i++) {
            graph->addVertex(i, 0);
        }
    }
    else{
        //TODO error
        std::cerr << "GraphFile not good";
    }
}

std::map<unsigned int, unsigned int> GraphFileConverter::makePartitionMap(const std::string &partitionFileName) {

    std::map<unsigned int, unsigned int> partitionMap;

    std::ifstream file(partitionFileName);

    if(file.good()) {
        std::string line;
        for (unsigned i = 1; std::getline(file, line); i++) {
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
GraphFileConverter::makeSubGraphs(const std::string &graphFileName,
                                  std::map<unsigned int, unsigned int> &partitionMap) {

    auto* subGraphs = new std::vector<Algora::DynamicDiGraph*>();

    for(auto &[_,__] : partitionMap){
        subGraphs->push_back(new Algora::DynamicDiGraph);
    }

    addSubVertices(partitionMap, nullptr);

    std::ifstream graphFile(graphFileName);
    if(graphFile.good()) {
        std::string line;
        std::getline(graphFile, line); //skip first line
        for (unsigned i = 1; std::getline(graphFile, line); i++) {

            std::stringstream graphSS(line);

            std::string neighbourString;

            while(std::getline(graphSS, neighbourString, ' ')){
                unsigned neighbourId = std::stoul(neighbourString);
                if(partitionMap[i] == partitionMap[neighbourId]){
                    subGraphs->at(partitionMap[i])->addArc(neighbourId, i, 0);
                }
            }

        }

        for(Algora::DynamicDiGraph* graph : *subGraphs){
            graph->applyNextDelta();
        }

        return subGraphs;

    }
    else{
        //TODO error
        std::cerr << "GraphFile not good";

        return subGraphs;
    }
}

void
GraphFileConverter::addSubVertices(const std::map<unsigned int, unsigned int> &partitionMap,
                                   Algora::DynamicDiGraph *graphs) {
    for(const auto &[id, partition]: partitionMap){
        graphs[partition].addVertex(id, 0);
    }
}

std::map<const Algora::Vertex *, Algora::Vertex *> *
GraphFileConverter::makeMainToOverlayMap(Algora::DynamicDiGraph *mainGraph, Algora::DynamicDiGraph *overlayGraph) {

    auto* vertexMap = new std::map<const Algora::Vertex*, Algora::Vertex*>();

    for(unsigned i = 1; i < mainGraph->getCurrentGraphSize()+1; i++){
        vertexMap->insert(mainGraph->getCurrentVertexForId(i), overlayGraph->getCurrentVertexForId(i));
    }
    return vertexMap;
}

std::map<const Algora::Vertex *, Algora::Vertex *> *
GraphFileConverter::makeInMap(Algora::DynamicDiGraph *overlayGraph, std::vector<Algora::DynamicDiGraph *> *subGraphs,
                              std::map<unsigned int, unsigned int> partitionMap) {
    auto* vertexMap = new std::map<const Algora::Vertex*, Algora::Vertex*>();

    for(unsigned i = 1; i < overlayGraph->getCurrentGraphSize()+1; i++){
        Algora::DynamicDiGraph* subGraph = subGraphs->at(partitionMap[i]);
        Algora::Vertex* subVertex = subGraph->getCurrentVertexForId(i);
        vertexMap->insert(overlayGraph->getCurrentVertexForId(i), subVertex);
    }
    return vertexMap;
}
