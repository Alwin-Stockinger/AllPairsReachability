//
// Created by alwin on 6/19/19.
//

#include <iostream>
#include <spawn.h>
#include <wait.h>
#include <fstream>
#include <cstring>

#include <graph.incidencelist/incidencelistgraph.h>

#include "Partitioner.h"

Algora::FastPropertyMap<unsigned long long>
Partitioner::handlePartitioning(unsigned long long int k, Algora::DiGraph *graph) {
    std::string kahipFileName = "forKahip";

    convertDiGraphToKahip(graph, kahipFileName);
    //TODO implement Kahip options
    pid_t pid;

    std::string kahipName = "kaffpa";
    std::string preconfig = "--preconfiguration=esocial";
    std::string seed = "--seed=877";

    std::string kahipInputFileName = "k";
    std::string kahipArgInputFileName = "--output_filename=" + kahipInputFileName;
    std::string kahipK = "--k=" + std::to_string(k);

    char* kahipArgv[] = {kahipName.data(), kahipFileName.data(), kahipK.data(), seed.data(),preconfig.data(), kahipArgInputFileName.data(), nullptr};
    char* const envp[]={nullptr};
    std::cout << "\nStarting Kahip with k=" + std::to_string(k) <<"\n--------------KAHIP OUTPUT----------------"<< std::endl;
    int kahipStatus = posix_spawn(&pid, kahipName.data(), nullptr, nullptr, kahipArgv, envp);

    if(kahipStatus != 0 || waitpid(pid, &kahipStatus, 0) == -1){
        if(kahipStatus == 2) std::cerr << strerror(kahipStatus) << ": " << kahipName << std::endl;
        throw std::runtime_error("kahip could not be executed");
    }
    std::cout << "------------------KAHIP OUTPUT END----------------"<<std::endl;
    return makePartitionMap(kahipInputFileName, graph);
}

Algora::FastPropertyMap<unsigned long long> Partitioner::makePartitionMap(const std::string &partitionFileName, Algora::DiGraph* graph) {


    std::map<unsigned long long, Algora::Vertex*> vertices;

    unsigned long long vertexI = 0ULL;
    graph->mapVertices([&vertices, &vertexI](Algora::Vertex* vertex){
        vertices.insert({vertexI++, vertex});
    });

    Algora::FastPropertyMap<unsigned long long> partitionMap;

    std::ifstream file(partitionFileName);

    if(file.good()) {
        std::string line;
        for (unsigned long long i = 0ULL; std::getline(file, line); i++) {
            partitionMap[vertices[i]] = std::stoul(line);
        }
    }
    else{
        //TODO error
        std::cerr << "PartitionFile not good" << std::endl;
        std::cerr << "Error: " << strerror(errno);
    }

    return partitionMap;
}

void Partitioner::convertDiGraphToKahip(Algora::DiGraph *graph, const std::string& outputFileName){
    std::map<unsigned long long int, std::map<unsigned long long int, unsigned long long int>> vertexMap = generateVertexMapFromGraph(graph);
    writeMapToFile(outputFileName, graph, vertexMap);
}

std::map<unsigned long long int, std::map<unsigned long long int, unsigned long long int>> Partitioner::generateVertexMapFromGraph(
        Algora::DiGraph *graph) {

    //Algora::FastPropertyMap<unsigned long long> algoraToKahipMap;
    //std::map <unsigned long long, Algora::Vertex*> kahipToAlgoraMap;

    std::map<unsigned long long , std::map<unsigned long long, unsigned long long>> vertexMap;

    graph->mapVertices([&vertexMap](Algora::Vertex* vertex){
        vertexMap[vertex->getId()];
    });

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

void Partitioner::writeMapToFile(const std::string &outFileName, Algora::DiGraph *graph,
                                        std::map<unsigned long long, std::map<unsigned long long, unsigned long long>> vertexMap) {


    std::ofstream outFile(outFileName, std::ios::trunc);

    unsigned long long arcSize = 0;

    for(const auto& [_,map]: vertexMap){
        arcSize += map.size();
    }
    arcSize /= 2;

    if(outFile.good() && outFile.is_open()){
        outFile << graph->getSize() << " " << arcSize << " 1\n";
        //outFile.flush();
    }

    for(auto& [head,map] : vertexMap){

        for( auto &[tail, weight] : map){

            unsigned long long kahipTailId = 1ULL;

            for(const auto &[algoraTailId,_] : vertexMap){
                if(algoraTailId == tail){
                    break;
                } else{
                    kahipTailId++;
                }
            }

            outFile << kahipTailId << " " << weight << " ";

        }
        outFile << "\n";
    }

    /*for(unsigned long long i = 0; i < graph->getSize(); i++){
        if(vertexMap.find(i) != vertexMap.end()){
            for(const auto &[tail, weight]: vertexMap[i]){
                outFile << tail + 1 << " " << weight << " ";
            }
        }
        outFile << "\n";
        //outFile.flush();
    }*/

    outFile.close();
}

std::map<std::string, Algora::FastPropertyMap<unsigned long long int>>
Partitioner::handleMultiPartitioning(Algora::DiGraph *diGraph, unsigned long long k, unsigned depth) {
    std::cout << "\nStarting Multi Level Partitioning with k=" << k << std::endl;

    unsigned long long id = 0ULL;

    std::map<std::string, Algora::FastPropertyMap<unsigned long long int>> returnMap;

    std::vector<Algora::DiGraph*> graphsToPartition {diGraph};
    diGraph->setName("0");

    // given graph should not be deleted!!!!
    std::vector<Algora::DiGraph*> allGraphs;

    for(unsigned level = depth; level != 0U - 1; level --){

        std::vector<Algora::DiGraph*> nextGraphs;

        for(Algora::DiGraph* partitionGraph : graphsToPartition){

            Algora::FastPropertyMap<unsigned long long> partition = handlePartitioning(k, partitionGraph);
            returnMap[std::to_string(id++)] = partition;

            std::vector<Algora::DiGraph*> builded = buildGraphs(partition, partitionGraph, k);

            nextGraphs.insert(std::end(nextGraphs), std::begin(builded), std::end(builded));
        }

        allGraphs.insert(std::end(allGraphs), std::begin(nextGraphs), std::end(nextGraphs));
        graphsToPartition = nextGraphs;
    }

    for( Algora::DiGraph* graph : allGraphs){
        delete graph;
    }

    std::cout << "Finished Multi Level Partitioning\n" << std::endl;

    return returnMap;
}


std::vector<Algora::DiGraph *>
Partitioner::buildGraphs(const Algora::FastPropertyMap<unsigned long long int> &partition, Algora::DiGraph *mainGraph,
                         const unsigned long long k) {

    std::vector<Algora::DiGraph*> subGraphs;

    for(auto i = 0ULL; i < k; i++){
        auto* graph = new Algora::IncidenceListGraph;

        subGraphs.push_back(graph);
    }

    Algora::FastPropertyMap<Algora::Vertex*> mainToSubMap;

    mainGraph->mapVertices([&subGraphs, partition, &mainToSubMap](Algora::Vertex* vertex){

        auto* subVertex = subGraphs.at(partition.getValue(vertex))->addVertex();
        mainToSubMap[vertex] = subVertex;
    });

    mainGraph->mapArcs([&subGraphs, partition, &mainToSubMap](Algora::Arc* arc){

        auto* mainHead = arc->getHead();
        auto * mainTail = arc->getTail();

        unsigned long long headPartition = partition.getValue(mainHead);
        unsigned long long tailPartition = partition.getValue(mainTail);

        Algora::Vertex* subTail = mainToSubMap[mainTail];
        Algora::Vertex* subHead = mainToSubMap[mainHead];


        if( headPartition == tailPartition){
            subGraphs.at(headPartition)->addArc(subTail, subHead);
        }
    });

    return subGraphs;
}
