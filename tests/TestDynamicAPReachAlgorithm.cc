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

TEST_P(TestDynamicAPReachAlgorithm, testTwoWayArc) {
    Algora::Vertex* vertex1 = dynamicDiGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex2 = dynamicDiGraph -> getCurrentVertexForId(2);

    dynamicDiGraph -> addArc(1,2,1);
    dynamicDiGraph -> applyNextDelta();

    ASSERT_TRUE(algorithm -> query(vertex1, vertex2));
    ASSERT_FALSE(algorithm -> query(vertex2, vertex1));

    dynamicDiGraph -> addArc(2,1,2);
    dynamicDiGraph -> applyNextDelta();

    ASSERT_TRUE(algorithm -> query(vertex2, vertex1));
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

TEST_P(TestDynamicAPReachAlgorithm, testArcRemove) {
    Algora::Vertex* vertex1 = dynamicDiGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex2 = dynamicDiGraph -> getCurrentVertexForId(2);

    dynamicDiGraph -> addArc(1,2,1);
    dynamicDiGraph -> applyNextDelta();

    ASSERT_TRUE(algorithm -> query(vertex1, vertex2));

    dynamicDiGraph -> removeArc(1,2,2);
    dynamicDiGraph -> applyNextDelta();

    ASSERT_FALSE(algorithm -> query(vertex1, vertex2));
}

TEST_P(TestDynamicAPReachAlgorithm, testMultipleArcWay) {
    Algora::Vertex* vertex1 = dynamicDiGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex3 = dynamicDiGraph -> getCurrentVertexForId(3);

    dynamicDiGraph -> addArc(1,2,1);
    dynamicDiGraph -> addArc(2,3,1);
    dynamicDiGraph -> applyNextDelta();

    ASSERT_TRUE(algorithm -> query(vertex1, vertex3));
}

TEST_P(TestDynamicAPReachAlgorithm, testVertexRemove){
    dynamicDiGraph -> removeVertex(1, 1);
    dynamicDiGraph->applyNextDelta();
}

TEST_P(TestDynamicAPReachAlgorithm, testMultipleWay){
    dynamicDiGraph->addVertex(4,1);

    dynamicDiGraph->addArc(1,2,1);
    dynamicDiGraph->addArc(2,3,1);
    dynamicDiGraph->addArc(3,4,1);
    dynamicDiGraph->addArc(1,4,1);
    dynamicDiGraph->applyNextDelta();

    Algora::Vertex* vertex1 = dynamicDiGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex4 = dynamicDiGraph -> getCurrentVertexForId(4);
    ASSERT_TRUE(algorithm -> query(vertex1, vertex4));
}

TEST_P(TestDynamicAPReachAlgorithm, testLongWay){
    dynamicDiGraph->addVertex(4,1);
    dynamicDiGraph->addVertex(5,1);
    dynamicDiGraph->addVertex(6,1);
    dynamicDiGraph->addVertex(7,1);
    dynamicDiGraph->addVertex(8,1);
    dynamicDiGraph->addVertex(9,1);

    dynamicDiGraph->addArc(1,2,1);
    dynamicDiGraph->addArc(2,3,1);
    dynamicDiGraph->addArc(3,4,1);
    dynamicDiGraph->addArc(4,5,1);
    dynamicDiGraph->addArc(5,6,1);
    dynamicDiGraph->addArc(6,7,1);
    dynamicDiGraph->addArc(7,8,1);
    dynamicDiGraph->addArc(8,9,1);

    dynamicDiGraph->applyNextDelta();

    Algora::Vertex* vertex1 = dynamicDiGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex5 = dynamicDiGraph -> getCurrentVertexForId(5);
    Algora::Vertex* vertex9 = dynamicDiGraph -> getCurrentVertexForId(9);
    ASSERT_TRUE(algorithm -> query(vertex5, vertex9));
    ASSERT_TRUE(algorithm -> query(vertex1, vertex9));
}