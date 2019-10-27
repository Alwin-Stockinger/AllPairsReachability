//
// Created by Alwin Stockinger.
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
