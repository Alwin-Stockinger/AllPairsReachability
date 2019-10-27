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

#ifndef ALLPAIRREACH_SIMPLEPARTITIONEDDAPREACHALGORITHMIMPLEMENTATION_H
#define ALLPAIRREACH_SIMPLEPARTITIONEDDAPREACHALGORITHMIMPLEMENTATION_H

#include <map>
#include <set>
#include <property/propertymap.h>

#include "DynamicAPReachAlgorithm.h"
#include "DynamicSSBasedDAPReachAlgorithm.h"
#include "PartitionedAPReachAlgorithm.h"
#include "SimplePartitionedDAPReachAlgorithm.h"

template <typename SubAlgorithm, typename OverlayAlgorithm = SubAlgorithm, bool propagatePartitionFunction = true>
class SimplePartitionedDAPReachAlgorithmImplementation : public SimplePartitionedDAPReachAlgorithm{

public:
    static_assert(std::is_base_of<DynamicAPReachAlgorithm,SubAlgorithm>::value, "Template Parameter has to inherit from DynamicAPAlgorithm");
    static_assert(std::is_base_of<DynamicAPReachAlgorithm,OverlayAlgorithm>::value, "Template Parameter has to inherit from DynamicAPAlgorithm");

    explicit SimplePartitionedDAPReachAlgorithmImplementation(const unsigned depth = 0U) : SimplePartitionedDAPReachAlgorithm(){
        this->depth=depth;
    };


    ~SimplePartitionedDAPReachAlgorithmImplementation() override = default;

private:

    DynamicAPReachAlgorithm *createOverlayAlgorithm() override {
        return new OverlayAlgorithm;
    }

    DynamicAPReachAlgorithm *createSubAlgorithm() override {
        if( depth > 0U){
            auto* newAlgo = new SimplePartitionedDAPReachAlgorithmImplementation<SubAlgorithm, OverlayAlgorithm, propagatePartitionFunction>(depth - 1);
            if(propagatePartitionFunction){
                newAlgo->setPartitionFunction(partitionFunction, k);
            }
            return newAlgo;
        } else{
            return new SubAlgorithm;
        }
    }
};


#endif //ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H
