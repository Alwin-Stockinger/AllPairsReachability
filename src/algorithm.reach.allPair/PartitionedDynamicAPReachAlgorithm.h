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

#ifndef ALLPAIRREACH_PARTITIONEDDYNAMICAPREACHALGORITHM_H
#define ALLPAIRREACH_PARTITIONEDDYNAMICAPREACHALGORITHM_H

#include "PartitionedAPReachAlgorithm.h"

#include "DynamicAPReachAlgorithm.h"

class PartitionedDynamicAPReachAlgorithm : public PartitionedAPReachAlgorithm{

public:

    ~PartitionedDynamicAPReachAlgorithm() override{
        for(auto &[_,algorithm] : graphToAlgorithmMap){
            delete(algorithm);
        }
    }

protected:

    virtual DynamicAPReachAlgorithm* createSubAlgorithm() = 0;

    void insertOverlayEdgeArcsOfNewOverlayVertex(Algora::Vertex* vertex) override;

    void insertOverlayEdgeArcs(Algora::DiGraph *subGraph) override;

    void removeOverlayEdgeArcs(Algora::DiGraph *subGraph) override;

    Algora::PropertyMap<DynamicAPReachAlgorithm*> graphToAlgorithmMap;

    void initializeChildStructures() override;
    void resetChildStructures() override ;

private:

};


#endif //ALLPAIRREACH_PARTITIONEDDYNAMICAPREACHALGORITHM_H
