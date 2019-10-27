//
// Created by alwin on 6/19/19.
//

#ifndef ALLPAIRREACH_PARTITIONER_H
#define ALLPAIRREACH_PARTITIONER_H

#include <map>

#include <graph/digraph.h>
#include <property/fastpropertymap.h>

class Partitioner {

public:
    static Algora::FastPropertyMap<unsigned long long> handlePartitioning(unsigned long long int k, Algora::DiGraph *graph);
    static std::map<std::string, Algora::FastPropertyMap<unsigned long long int>>
        handleMultiPartitioning(Algora::DiGraph *diGraph, unsigned long long k, unsigned depth);
private:
    static void convertDiGraphToKahip(Algora::DiGraph *graph, const std::string &outputFileName);
    static Algora::FastPropertyMap< unsigned long long> makePartitionMap(const std::string &partitionFileName, Algora::DiGraph* graph);

    static std::map<unsigned long long int, std::map<unsigned long long int, unsigned long long int>>
    generateVertexMapFromGraph(Algora::DiGraph *graph);

    static void writeMapToFile(const std::string &outFileName, Algora::DiGraph *graph,
                        std::map<unsigned long long int, std::map<unsigned long long int, unsigned long long int>> vertexMap);


    static std::vector<Algora::DiGraph *>
    buildGraphs(const Algora::FastPropertyMap<unsigned long long int> &partition, Algora::DiGraph *mainGraph,
                unsigned long long int k);
};


#endif //ALLPAIRREACH_PARTITIONER_H
