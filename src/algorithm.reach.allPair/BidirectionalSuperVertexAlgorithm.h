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

#ifndef ALLPAIRREACH_BIDIRECTIONALSUPERVERTEXALGORITHM_H
#define ALLPAIRREACH_BIDIRECTIONALSUPERVERTEXALGORITHM_H


#include "PartitionedAPReachAlgorithm.h"
#include "SimplePartitionedDAPReachAlgorithmImplementation.h"


class BidirectionalSuperVertexAlgorithm
        : public PartitionedDynamicAPReachAlgorithm {

public:


    bool query(Algora::Vertex *start, const Algora::Vertex *end) override;


    ~BidirectionalSuperVertexAlgorithm() override;

    Algora::PropertyMap<Algora::Vertex*> sourceSuperVertices;
    Algora::PropertyMap<Algora::Vertex*> destinationSuperVertices;

    void onArcAdd(Algora::Arc *arc) override;

    std::string getBaseName() override;

    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;

    void setStepSize(unsigned long long newSteps){
        this->stepSize = newSteps;
    }

protected:

    void initializeChildStructures() override;

    void resetChildStructures() override;

private:

    void registerOnOverlay();
    void deregisterOnOverlay();

    void generateSuperVertices();
    void resetSuperStructure();

    std::vector<Algora::Vertex*> delayedVertices;

    unsigned long long stepSize = 1ULL;

    bool bidirectionalOverlaySearch(Algora::Vertex* source, Algora::Vertex* target);

};


#endif //ALLPAIRREACH_BIDIRECTIONALSUPERVERTEXALGORITHM_H
