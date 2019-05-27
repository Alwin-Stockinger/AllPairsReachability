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

    static void convertDiToUnDi(const std::string &inName, const std::string &outName);

    static Algora::DynamicDiGraph * readGraph(const std::string &fileName);

    static Algora::DynamicDiGraph *makeOverlay(const std::string &graphFileName,
                                               std::map<unsigned int, unsigned int> &partitionMap);

    static void addVertices(const std::string &graphFileName, Algora::DynamicDiGraph *graph);

    static std::map<unsigned int, unsigned int> makePartitionMap(const std::string &partitionFileName);

    static std::vector<Algora::DynamicDiGraph *> *
    makeSubGraphs(const std::string &graphFileName, std::map<unsigned int, unsigned int> &partitionMap);

    static void addSubVertices(const std::map<unsigned int, unsigned int> &partitionMap,
                               std::vector<Algora::DynamicDiGraph*> *graphs);

    static std::map<const Algora::Vertex *, Algora::Vertex *> *
    makeMainToOverlayMap(Algora::DynamicDiGraph *mainGraph, Algora::DynamicDiGraph *overlayGraph);

    static std::map<const Algora::Vertex *, Algora::Vertex *> *
    makeInMap(Algora::DynamicDiGraph *overlayGraph, std::vector<Algora::DynamicDiGraph *> *subGraphs,
              std::map<unsigned int, unsigned int> partitionMap);
};


#endif //ALLPAIRREACH_GRAPHFILECONVERTER_H
