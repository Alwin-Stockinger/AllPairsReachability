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

#include "TwoWayFactory.h"
#include "../../algorithm.reach.allPair/TwoWayBFSAPReachAlgorithm.h"

std::vector<DynamicAPReachAlgorithm *> TwoWayFactory::getStaticBFSAlgorithms() {
    return std::vector<DynamicAPReachAlgorithm *>();
}

std::vector<DynamicAPReachAlgorithm *> TwoWayFactory::getStaticDFSAlgorithms() {
    return std::vector<DynamicAPReachAlgorithm *>();
}

std::vector<DynamicAPReachAlgorithm *> TwoWayFactory::getCachingBFSAlgorithms() {
    return std::vector<DynamicAPReachAlgorithm *>();
}

std::vector<DynamicAPReachAlgorithm *> TwoWayFactory::getCachingDFSAlgorithms() {
    return std::vector<DynamicAPReachAlgorithm *>();
}

std::vector<DynamicAPReachAlgorithm *> TwoWayFactory::getLazyBFSAlgorithms() {
    return std::vector<DynamicAPReachAlgorithm *>();
}

std::vector<DynamicAPReachAlgorithm *> TwoWayFactory::getLazyDFSAlgorithms() {
    return std::vector<DynamicAPReachAlgorithm *>();
}

std::vector<DynamicAPReachAlgorithm *> TwoWayFactory::getSimpleISSRAlgorithms() {
    return std::vector<DynamicAPReachAlgorithm *>();
}

std::vector<DynamicAPReachAlgorithm *> TwoWayFactory::getSimpleESTAlgorithms() {
    return std::vector<DynamicAPReachAlgorithm *>();
}

std::vector<DynamicAPReachAlgorithm *> TwoWayFactory::getMLESTAlgorithms() {
    return std::vector<DynamicAPReachAlgorithm *>();
}

std::vector<DynamicAPReachAlgorithm *> TwoWayFactory::getESTAlgorithms() {
    return std::vector<DynamicAPReachAlgorithm *>();
}

std::vector<DynamicAPReachAlgorithm *> TwoWayFactory::getOldESTAlgorithms() {
    return std::vector<DynamicAPReachAlgorithm *>();
}

std::vector<DynamicAPReachAlgorithm *> TwoWayFactory::getAlgorithms() {

    std::vector<DynamicAPReachAlgorithm*> algorithmVector;
    for(unsigned long long steps = minSteps; steps <= maxSteps ; steps++){
        auto* algorithm = new TwoWayBFSAPReachAlgorithm();
        algorithm->setStepSize(steps);
        algorithmVector.push_back(algorithm);
    }
    return algorithmVector;
}
