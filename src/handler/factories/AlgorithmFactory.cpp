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

#include "AlgorithmFactory.h"

std::vector<DynamicAPReachAlgorithm *> AlgorithmFactory::getAlgorithms() {
    std::vector<DynamicAPReachAlgorithm*> algorithms;


    for(const std::string& algorithmName : algorithmNames){

        std::vector<DynamicAPReachAlgorithm*> newAlgorithms;

        if(algorithmName == "StaticBFS") {
            newAlgorithms = getStaticBFSAlgorithms();
        }
        else if(algorithmName == "StaticDFS") {
            newAlgorithms = getStaticDFSAlgorithms();
        }
        else if( algorithmName == "LazyDFS") {
            newAlgorithms = getLazyDFSAlgorithms();
        }
        else if( algorithmName == "LazyBFS") {
            newAlgorithms = getLazyBFSAlgorithms();
        }
        else if( algorithmName == "CachingDFS") {
            newAlgorithms = getCachingDFSAlgorithms();
        }
        else if( algorithmName == "CachingBFS") {
            newAlgorithms = getCachingBFSAlgorithms();
        }
        else if( algorithmName == "SimpleInc") {
            newAlgorithms = getSimpleISSRAlgorithms();
        }
        else if( algorithmName == "ESTreeML") {
            newAlgorithms = getMLESTAlgorithms();
        }
        else if( algorithmName == "OldESTree") {
            newAlgorithms = getOldESTAlgorithms();
        }
        else if( algorithmName == "ESTreeQ") {
            newAlgorithms = getESTAlgorithms();
        }
        else if( algorithmName == "SimpleESTree") {
            newAlgorithms = getSimpleESTAlgorithms();
        }
        else{
            throw std::invalid_argument(algorithmName + " is not a viable algorithm");
        }

        algorithms.insert(algorithms.end(), newAlgorithms.begin(), newAlgorithms.end());
    }
    return algorithms;
}
