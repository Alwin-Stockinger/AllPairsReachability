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

#ifndef ALLPAIRREACH_TESTDYNAMICAPREACHALGORITHM_H
#define ALLPAIRREACH_TESTDYNAMICAPREACHALGORITHM_H

#include <graph.dyn/dynamicdigraph.h>

#include "gtest/gtest.h"

#include "../src/algorithm.reach.allPair/DynamicAPReachAlgorithm.h"

class TestDynamicAPReachAlgorithm : public testing::TestWithParam<DynamicAPReachAlgorithm*>{

public:
    DynamicAPReachAlgorithm *algorithm;

    Algora::DynamicDiGraph *dynamicDiGraph;

    ~TestDynamicAPReachAlgorithm() override{
        delete algorithm;
        delete dynamicDiGraph;
    }

public:
    void TearDown() override {
    }

    void SetUp() override {
        dynamicDiGraph = new Algora::DynamicDiGraph;

        dynamicDiGraph->addVertex(1, 0);
        dynamicDiGraph->addVertex(2, 0);
        dynamicDiGraph->addVertex(3, 0);

        dynamicDiGraph->applyNextDelta();

        algorithm = GetParam();
        algorithm -> setGraph(dynamicDiGraph->getDiGraph());
    }
};


#endif //ALLPAIRREACH_TESTDYNAMICAPREACHALGORITHM_H
