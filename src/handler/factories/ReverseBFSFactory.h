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

#ifndef ALLPAIRREACH_REVERSEBFSFACTORY_H
#define ALLPAIRREACH_REVERSEBFSFACTORY_H


#include "AlgorithmFactory.h"
#include "PartitionedFactory.h"

class ReverseBFSFactory : public PartitionedFactory{
public:
    std::vector<DynamicAPReachAlgorithm *> getAlgorithms() override;


    void setAdvancedBFSO(bool b) {
        this->advancedBFSO = b;
    }

    void setSimpleBFSO(bool b) {
        this->simpleBFSO = b;
    }


    void setMinSteps(unsigned long long newMinSteps){
        minSteps = newMinSteps;
    }

    void setMaxSteps(unsigned long long newMaxSteps){
        maxSteps = newMaxSteps;
    }

    void setFullyReverseBFS(bool b);

private:

    bool fullyReverse = false;

    unsigned long long minSteps = 2ULL;
    unsigned long long maxSteps = 2ULL;

    bool advancedBFSO = false;
    bool simpleBFSO = false;

private:

    std::vector<DynamicAPReachAlgorithm *> getStaticBFSAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getStaticDFSAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getCachingBFSAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getCachingDFSAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getLazyBFSAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getLazyDFSAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getSimpleISSRAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getSimpleESTAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getMLESTAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getESTAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getOldESTAlgorithms() override;

};


#endif //ALLPAIRREACH_REVERSEBFSFACTORY_H
