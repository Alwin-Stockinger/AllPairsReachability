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

#ifndef ALLPAIRREACH_SIMPLEPARTITIONEDDAPREACHALGORITHM_H
#define ALLPAIRREACH_SIMPLEPARTITIONEDDAPREACHALGORITHM_H


#include "PartitionedAPReachAlgorithm.h"
#include "PartitionedDynamicAPReachAlgorithm.h"

class SimplePartitionedDAPReachAlgorithm : public PartitionedDynamicAPReachAlgorithm{

public:

    bool query(Algora::Vertex *start, const Algora::Vertex *end) override;

    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;

    std::string getBaseName() override  {

        std::string retName = "Simple";
        retName += "(" + getPartitionConfiguration() + ")";
        retName += ": " + graphToAlgorithmMap.begin()->second->getBaseName();

        retName += "(" + overlayAlgorithm->getBaseName() + ")";

        return retName;
    }

    ~SimplePartitionedDAPReachAlgorithm() override;

protected:
    virtual DynamicAPReachAlgorithm* createOverlayAlgorithm() = 0;

    void initializeChildStructures() override;

    void resetChildStructures() override;

private:
    DynamicAPReachAlgorithm* overlayAlgorithm = nullptr;
};


#endif //ALLPAIRREACH_SIMPLEPARTITIONEDDAPREACHALGORITHM_H
