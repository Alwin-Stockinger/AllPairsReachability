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
#ifndef ALLPAIRREACH_PARTITIONEDFACTORY_H
#define ALLPAIRREACH_PARTITIONEDFACTORY_H


#include <property/fastpropertymap.h>
#include "AlgorithmFactory.h"
#include "../../partition/Partitioner.h"
#include "../../algorithm.reach.allPair/PartitionedAPReachAlgorithm.h"

using PartFunc = std::function<Algora::FastPropertyMap<unsigned long long>(unsigned long long int, Algora::DiGraph*)>;

class PartitionedFactory : public AlgorithmFactory{

public:
    void setOverlayNames(const std::vector<std::string> &overlayNames);

    void setKMin(unsigned long long int kMin);

    void setKMax(unsigned long long int kMax);

    void setDepthMin(unsigned long long int depthMin);

    void setDepthMax(unsigned long long int depthMax);

    void setPartitionFunction(const PartFunc &partitionFunction);

    void setExponentialK(bool exponentialK);

    void setRepartitionThreshold(unsigned long long int threshold);

    void setRandomVertexAdditions(bool b){
        randomVertexAdditions = b;
    }

protected:

    void configureAlgorithm(PartitionedAPReachAlgorithm* algorithm){
        algorithm->setPartitionFunction(partitionFunction);
        algorithm->setRepartitionThreshold(repartitionThreshold);
        algorithm->setRandomVertexAdditions(randomVertexAdditions);
    }

    PartFunc partitionFunction = Partitioner::handlePartitioning;
    std::vector<std::string> overlayNames;

    unsigned long long kMin = 2;
    unsigned long long kMax = 2;
    bool exponentialK = true;

    unsigned long long depthMin = 0;
    unsigned long long depthMax = 0;

    unsigned long long repartitionThreshold = 0ULL;

    bool randomVertexAdditions;
};


#endif //ALLPAIRREACH_PARTITIONEDFACTORY_H
