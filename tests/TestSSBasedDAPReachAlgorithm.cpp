//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_TESTDYNAMICAPREACHALGORITHM_CPP
#define ALLPAIRREACH_TESTDYNAMICAPREACHALGORITHM_CPP

#include <graph.dyn/dynamicdigraph.h>
#include <algorithm.reach/staticdfsssreachalgorithm.h>
#include <algorithm.reach/staticbfsssreachalgorithm.h>
#include <algorithm.reach/cachingbfsssreachalgorithm.h>
#include <algorithm.reach/cachingdfsssreachalgorithm.h>
#include <algorithm.reach/lazydfsssreachalgorithm.h>
#include <algorithm.reach/simpleincssreachalgorithm.h>
#include <algorithm.reach/lazybfsssreachalgorithm.h>
#include <algorithm.reach.es/estree-queue.h>
#include <algorithm.reach.es/estree-bqueue.h>
#include <algorithm.reach.es/estree-ml.h>
#include <algorithm.reach.es/simpleestree.h>

#include "gtest/gtest.h"

#include "../src/algorithm.reach.allPair/DynamicAPReachAlgorithm.h"

template <typename T>
class TestSSBasedAPReachAlgorithm : public testing::Test{

public:
    DynamicAPReachAlgorithm<T> *algorithm;

    Algora::DynamicDiGraph *dynamicDiGraph;

    ~TestSSBasedAPReachAlgorithm() override{
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

        algorithm = new T();
        algorithm -> setGraph(dynamicDiGraph->getDiGraph());
    }
};

using TestTypes = ::testing::Types<Algora::StaticDFSSSReachAlgorithm, Algora::StaticBFSSSReachAlgorithm, Algora::CachingBFSSSReachAlgorithm, Algora::CachingDFSSSReachAlgorithm, Algora::LazyDFSSSReachAlgorithm, Algora::LazyBFSSSReachAlgorithm, Algora::SimpleIncSSReachAlgorithm, Algora::ESTreeQ, Algora::OldESTree, Algora::ESTreeML, Algora::SimpleESTree>;
TYPED_TEST_SUITE(TestSSBasedAPReachAlgorithm, TestTypes);

TYPED_TEST(TestSSBasedAPReachAlgorithm, testNoArc) {
    Algora::Vertex* vertex1 = this->dynamicDiGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex2 = this->dynamicDiGraph -> getCurrentVertexForId(2);

    ASSERT_FALSE(this->algorithm -> query(vertex1, vertex2));
}

TYPED_TEST(TestSSBasedAPReachAlgorithm, testArc) {
    Algora::Vertex* vertex1 = this->dynamicDiGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex2 = this->dynamicDiGraph -> getCurrentVertexForId(2);

    this->dynamicDiGraph -> addArc(1,2,1);
    this->dynamicDiGraph -> applyNextDelta();

    ASSERT_TRUE(this->algorithm -> query(vertex1, vertex2));
}

TYPED_TEST(TestSSBasedAPReachAlgorithm, testTwoWayArc) {
    Algora::Vertex* vertex1 = this->dynamicDiGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex2 = this->dynamicDiGraph -> getCurrentVertexForId(2);

    this->dynamicDiGraph -> addArc(1,2,1);
    this->dynamicDiGraph -> applyNextDelta();

    ASSERT_TRUE(this->algorithm -> query(vertex1, vertex2));
    ASSERT_FALSE(this->algorithm -> query(vertex2, vertex1));

    this->dynamicDiGraph -> addArc(2,1,2);
    this->dynamicDiGraph -> applyNextDelta();

    ASSERT_TRUE(this->algorithm -> query(vertex2, vertex1));
}

TYPED_TEST(TestSSBasedAPReachAlgorithm, testwrongArc) {
    Algora::Vertex* vertex1 = this->dynamicDiGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex3 = this->dynamicDiGraph -> getCurrentVertexForId(3);

    this->dynamicDiGraph -> addArc(1,2,1);
    this->dynamicDiGraph -> applyNextDelta();

    ASSERT_FALSE(this->algorithm -> query(vertex1, vertex3));
}

TYPED_TEST(TestSSBasedAPReachAlgorithm, testArcInWrongDirection) {
    Algora::Vertex* vertex1 = this->dynamicDiGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex2 = this->dynamicDiGraph -> getCurrentVertexForId(2);

    this->dynamicDiGraph -> addArc(2,1,1);
    this->dynamicDiGraph -> applyNextDelta();

    ASSERT_FALSE(this->algorithm -> query(vertex1, vertex2));
}

TYPED_TEST(TestSSBasedAPReachAlgorithm, testArcRemove) {
    Algora::Vertex* vertex1 = this->dynamicDiGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex2 = this->dynamicDiGraph -> getCurrentVertexForId(2);

    this->dynamicDiGraph -> addArc(1,2,1);
    this->dynamicDiGraph -> applyNextDelta();

    ASSERT_TRUE(algorithm -> query(vertex1, vertex2));

    dynamicDiGraph -> removeArc(1,2,2);
    dynamicDiGraph -> applyNextDelta();

    ASSERT_FALSE(algorithm -> query(vertex1, vertex2));
}

TYPED_TEST(TestSSBasedAPReachAlgorithm, testMultipleArcWay) {
    Algora::Vertex* vertex1 = dynamicDiGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex3 = dynamicDiGraph -> getCurrentVertexForId(3);

    dynamicDiGraph -> addArc(1,2,1);
    dynamicDiGraph -> addArc(2,3,1);
    dynamicDiGraph -> applyNextDelta();

    ASSERT_TRUE(algorithm -> query(vertex1, vertex3));
}

TYPED_TEST(TestSSBasedAPReachAlgorithm, testVertexRemove){
    dynamicDiGraph -> removeVertex(1, 1);
    dynamicDiGraph->applyNextDelta();
}



#endif //ALLPAIRREACH_TESTDYNAMICAPREACHALGORITHM_CPP
