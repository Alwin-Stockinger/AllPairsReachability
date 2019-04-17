//
// Created by Alwin Stockinger.
//

#include "TestDynamicAPReachAlgorithm.h"

TEST_P(TestDynamicAPReachAlgorithm, testNoArc) {
    Algora::Vertex* vertex1 = dynamicDiGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex2 = dynamicDiGraph -> getCurrentVertexForId(2);

    ASSERT_FALSE(algorithm -> query(vertex1, vertex2));
}

TEST_P(TestDynamicAPReachAlgorithm, testArc) {
    Algora::Vertex* vertex1 = dynamicDiGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex2 = dynamicDiGraph -> getCurrentVertexForId(2);

    dynamicDiGraph -> addArc(1,2,1);
    dynamicDiGraph -> applyNextDelta();

    ASSERT_TRUE(algorithm -> query(vertex1, vertex2));
}

TEST_P(TestDynamicAPReachAlgorithm, testwrongArc) {
    Algora::Vertex* vertex1 = dynamicDiGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex3 = dynamicDiGraph -> getCurrentVertexForId(3);

    dynamicDiGraph -> addArc(1,2,1);
    dynamicDiGraph -> applyNextDelta();

    ASSERT_FALSE(algorithm -> query(vertex1, vertex3));
}

TEST_P(TestDynamicAPReachAlgorithm, testArcInWrongDirection) {
    Algora::Vertex* vertex1 = dynamicDiGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex2 = dynamicDiGraph -> getCurrentVertexForId(2);

    dynamicDiGraph -> addArc(2,1,1);
    dynamicDiGraph -> applyNextDelta();

    ASSERT_FALSE(algorithm -> query(vertex1, vertex2));
}