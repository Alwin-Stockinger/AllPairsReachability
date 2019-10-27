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

#ifndef ALLPAIRREACH_REVERSEBFSPARTITIONEDAPREACHALGORITHM_H
#define ALLPAIRREACH_REVERSEBFSPARTITIONEDAPREACHALGORITHM_H


#include "PartitionedAPReachAlgorithm.h"

class ReverseBFSPartitionedAPReachAlgorithm : public PartitionedAPReachAlgorithm{

public:

    bool query(Algora::Vertex *start, const Algora::Vertex *end) override;


    void setSetRemovals( bool b){
        this->setRemovals = b;
    }

    std::string getBaseName() override;

protected:

    void initializeChildStructures() override;

    void insertOverlayEdgeArcs(Algora::DiGraph *subGraph) override;

    void insertOverlayEdgeArcsOfNewOverlayVertex(Algora::Vertex *vertex) override;

    void removeOverlayEdgeArcs(Algora::DiGraph *subGraph) override;

    void resetChildStructures() override;

    void addVertex(Algora::Vertex *lazyVertex, Algora::DiGraph *subGraph) override;

    bool normalOverlayBFS(std::unordered_set<const Algora::Vertex*> sourceBorders, std::unordered_set<Algora::Vertex*> targetBorders);

    bool bidirectionalOverlayBFS(const std::unordered_set<const Algora::Vertex*>& sourceBorders,const std::unordered_set<Algora::Vertex*>& targetBorders);

public:
    void onVertexAdd(Algora::Vertex *vertex) override;

    void onVertexRemove(Algora::Vertex *vertex) override;

    void setStepSize(unsigned long long newStepSize){
        this->stepSize = newStepSize;
    }

    void setBidirectional(bool b){
        this->bidirectional = b;
    }

public:
    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;

private:
    Algora::PropertyMap<Algora::Vertex*> subToMainMap; //can't use FastPropertyMap because vertices of subgraphs could have same id

    bool setRemovals = true;

    bool bidirectional = false;

    unsigned long long stepSize = 0ULL;

};


#endif //ALLPAIRREACH_REVERSEBFSPARTITIONEDAPREACHALGORITHM_H
