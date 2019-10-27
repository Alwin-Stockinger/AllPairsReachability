//
// Copyright (c) 2019 : Alwin Stockinger
//
//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <https://www.gnu.org/licenses/>.
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
