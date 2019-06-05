//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_GRAPHFILECONVERTER_H
#define ALLPAIRREACH_GRAPHFILECONVERTER_H


#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <graph/digraph.h>
#include <graph.dyn/dynamicdigraph.h>

class GraphFileConverter {
public:

    static void convertKahipDiToUnDi(const std::string &inName, const std::string &outName);

    static Algora::DynamicDiGraph * readGraph(const std::string &fileName);

    static Algora::DynamicDiGraph *makeOverlay(const Algora::DynamicDiGraph &mainGraph,
                                               const std::map<unsigned long long, unsigned long long> &partitionMap);

    static void addVertices(const Algora::DynamicDiGraph &oldGraph, Algora::DynamicDiGraph *newGraph);

    static std::map<unsigned long long, unsigned long long> makePartitionMap(const std::string &partitionFileName);

    static std::vector<Algora::DynamicDiGraph *> *
    makeSubGraphs(const Algora::DynamicDiGraph& mainGraph, const std::map<unsigned long long, unsigned long long> &partitionMap);

    static void addSubVertices(const std::map<unsigned long long, unsigned long long> &partitionMap,
                               std::vector<Algora::DynamicDiGraph*> *graphs);

    static std::map<const Algora::Vertex *, Algora::Vertex *> *
    makeMainToOverlayMap(const Algora::DynamicDiGraph& mainGraph,const Algora::DynamicDiGraph *overlayGraph);

    static std::map<const Algora::Vertex *, Algora::Vertex *> *
    makeInMap(Algora::DynamicDiGraph *overlayGraph, std::vector<Algora::DynamicDiGraph *> *subGraphs,
              std::map<unsigned long long int, unsigned long long int> partitionMap);


    static std::map<unsigned long long int, std::map<unsigned long long int, unsigned long long int>>
    generateVertexMapFromGraph(Algora::DiGraph *graph);

    static void convertDiGraphToKahip(Algora::DiGraph *graph, const std::string &outputFileName);

private:
    static void writeMapToFile(const std::string &outFileName, Algora::DiGraph *graph,
                               std::map<unsigned long long, std::map<unsigned long long, unsigned long long>> vertexMap);


};


#endif //ALLPAIRREACH_GRAPHFILECONVERTER_H
