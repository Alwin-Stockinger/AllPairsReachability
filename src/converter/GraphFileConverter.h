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

#ifndef ALLPAIRREACH_GRAPHFILECONVERTER_H
#define ALLPAIRREACH_GRAPHFILECONVERTER_H


#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <graph/digraph.h>
#include <graph.dyn/dynamicdigraph.h>
#include <property/fastpropertymap.h>

class GraphFileConverter {
public:



    static Algora::DynamicDiGraph * readGraph(const std::string &fileName);

    static Algora::DynamicDiGraph *makeOverlay(const Algora::DynamicDiGraph &mainGraph,
                                               const std::map<unsigned long long, unsigned long long> &partitionMap);

    static void addVertices(const Algora::DynamicDiGraph &oldGraph, Algora::DynamicDiGraph *newGraph);



    static std::vector<Algora::DynamicDiGraph *> *
    makeSubGraphs(const Algora::DynamicDiGraph& mainGraph, const std::map<unsigned long long, unsigned long long> &partitionMap);

    static void addSubVertices(const std::map<unsigned long long, unsigned long long> &partitionMap,
                               std::vector<Algora::DynamicDiGraph*> *graphs);
};


#endif //ALLPAIRREACH_GRAPHFILECONVERTER_H
