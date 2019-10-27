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

#ifndef ALLPAIRREACH_ALGORITHMFACTORY_H
#define ALLPAIRREACH_ALGORITHMFACTORY_H


#include <utility>

#include "../../algorithm.reach.allPair/DynamicAPReachAlgorithm.h"

class AlgorithmFactory {
public:

    void setAlgorithmNames(std::vector<std::string> newAlgorithmNames){
        algorithmNames = std::move(newAlgorithmNames);
    }

    virtual std::vector<DynamicAPReachAlgorithm*> getAlgorithms();

protected:

    virtual std::vector <DynamicAPReachAlgorithm*> getStaticBFSAlgorithms() = 0;
    virtual std::vector <DynamicAPReachAlgorithm*> getStaticDFSAlgorithms() = 0;
    virtual std::vector <DynamicAPReachAlgorithm*> getCachingBFSAlgorithms() = 0;
    virtual std::vector <DynamicAPReachAlgorithm*> getCachingDFSAlgorithms() = 0;
    virtual std::vector <DynamicAPReachAlgorithm*> getLazyBFSAlgorithms() = 0;
    virtual std::vector <DynamicAPReachAlgorithm*> getLazyDFSAlgorithms() = 0;
    virtual std::vector <DynamicAPReachAlgorithm*> getSimpleISSRAlgorithms() = 0;
    virtual std::vector <DynamicAPReachAlgorithm*> getSimpleESTAlgorithms() = 0;
    virtual std::vector <DynamicAPReachAlgorithm*> getMLESTAlgorithms() = 0;
    virtual std::vector <DynamicAPReachAlgorithm*> getESTAlgorithms() = 0;
    virtual std::vector <DynamicAPReachAlgorithm*> getOldESTAlgorithms() = 0;

protected:
    std::vector<std::string> algorithmNames;


};


#endif //ALLPAIRREACH_ALGORITHMFACTORY_H
