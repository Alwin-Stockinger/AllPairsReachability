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

#include "ReverseBFSFactory.h"
#include "../../algorithm.reach.allPair/ReverseBFSPartitionedAPReachAlgorithm.h"


std::vector<DynamicAPReachAlgorithm *> ReverseBFSFactory::getAlgorithms() {
    std::vector<DynamicAPReachAlgorithm*> algorithms;
    for(unsigned long long k = kMin; k <= kMax ;  [this, &k]() {
        exponentialK ? (k *= 2ULL) : (k++);
    }()){
        if (fullyReverse) for(unsigned long long steps = minSteps; steps <= maxSteps; steps++){
                auto *algorithm = new ReverseBFSPartitionedAPReachAlgorithm;

                configureAlgorithm(algorithm);
                algorithm->setK(k);
                algorithm->setSetRemovals(true);
                algorithm->setStepSize(steps);
                algorithm->setBidirectional(fullyReverse);

                algorithms.push_back(algorithm);
        }
        else{
            if (advancedBFSO) {
                auto *algorithm = new ReverseBFSPartitionedAPReachAlgorithm;

                configureAlgorithm(algorithm);
                algorithm->setK(k);
                algorithm->setSetRemovals(true);

                algorithms.push_back(algorithm);
            }
            if (simpleBFSO) {
                auto *algorithm = new ReverseBFSPartitionedAPReachAlgorithm;

                configureAlgorithm(algorithm);
                algorithm->setK(k);
                algorithm->setSetRemovals(false);

                algorithms.push_back(algorithm);
            }
        }
    }
    return algorithms;
}

void ReverseBFSFactory::setFullyReverseBFS(bool b) {
    fullyReverse = b;
}

std::vector<DynamicAPReachAlgorithm *> ReverseBFSFactory::getStaticBFSAlgorithms() {
    throw std::logic_error("Can't make SS-algorithm for Reverse BFS");
}

std::vector<DynamicAPReachAlgorithm *> ReverseBFSFactory::getStaticDFSAlgorithms() {
    throw std::logic_error("Can't make SS-algorithm for Reverse BFS");
}

std::vector<DynamicAPReachAlgorithm *> ReverseBFSFactory::getCachingBFSAlgorithms() {
    throw std::logic_error("Can't make SS-algorithm for Reverse BFS");
}

std::vector<DynamicAPReachAlgorithm *> ReverseBFSFactory::getCachingDFSAlgorithms() {
    throw std::logic_error("Can't make SS-algorithm for Reverse BFS");
}

std::vector<DynamicAPReachAlgorithm *> ReverseBFSFactory::getLazyBFSAlgorithms() {
    throw std::logic_error("Can't make SS-algorithm for Reverse BFS");
}

std::vector<DynamicAPReachAlgorithm *> ReverseBFSFactory::getLazyDFSAlgorithms() {
    throw std::logic_error("Can't make SS-algorithm for Reverse BFS");
}

std::vector<DynamicAPReachAlgorithm *> ReverseBFSFactory::getSimpleISSRAlgorithms() {
    throw std::logic_error("Can't make SS-algorithm for Reverse BFS");
}

std::vector<DynamicAPReachAlgorithm *> ReverseBFSFactory::getSimpleESTAlgorithms() {
    throw std::logic_error("Can't make SS-algorithm for Reverse BFS");
}

std::vector<DynamicAPReachAlgorithm *> ReverseBFSFactory::getMLESTAlgorithms() {
    throw std::logic_error("Can't make SS-algorithm for Reverse BFS");}

std::vector<DynamicAPReachAlgorithm *> ReverseBFSFactory::getESTAlgorithms() {
    throw std::logic_error("Can't make SS-algorithm for Reverse BFS");}

std::vector<DynamicAPReachAlgorithm *> ReverseBFSFactory::getOldESTAlgorithms() {
    throw std::logic_error("Can't make SS-algorithm for Reverse BFS");}
