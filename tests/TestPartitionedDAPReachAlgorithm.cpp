//
// Created by Alwin Stockinger.
//

#include "../src/algorithm.reach.allPair/PartitionedDAPReachAlgorithmImplementation.h"
#include "../src/algorithm.reach.allPair/SSBasedDAPReachAlgorithm.h"

#include <gtest/gtest.h>

#include <algorithm.reach/staticbfsssreachalgorithm.h>
#include <graph.dyn/dynamicdigraph.h>
#include <algorithm.reach/staticdfsssreachalgorithm.h>
#include <algorithm.reach/cachingbfsssreachalgorithm.h>
#include <algorithm.reach/cachingdfsssreachalgorithm.h>
#include <algorithm.reach/lazydfsssreachalgorithm.h>
#include <algorithm.reach/lazybfsssreachalgorithm.h>
#include <algorithm.reach/simpleincssreachalgorithm.h>
#include <algorithm.reach.es/estree-queue.h>
#include <algorithm.reach.es/estree-bqueue.h>
#include <algorithm.reach.es/estree-ml.h>
#include <algorithm.reach.es/simpleestree.h>


template <typename T>
class TestPartitionedDAPReachAlgorithm : public testing::Test{

protected:
    PartitionedDAPReachAlgorithmImplementation<T>* algorithm = nullptr;

    Algora::DynamicDiGraph* mainGraph = nullptr;

    ~TestPartitionedDAPReachAlgorithm() override{
        delete algorithm;

        delete mainGraph;
    }

public:
    void TearDown() override {
    }

    void SetUp() override {

        mainGraph = new Algora::DynamicDiGraph;

        mainGraph->addVertex(0, 0);
        mainGraph->addVertex(1, 0);
        mainGraph->addVertex(2, 0);
        mainGraph->addVertex(3, 0);
        mainGraph->addVertex(4, 0);
        mainGraph->addVertex(5, 0);
        mainGraph->addVertex(6, 0);
        mainGraph->addVertex(7, 0);
        mainGraph->addArc(2,0,0);
        mainGraph->addArc(1,2,0);
        mainGraph->addArc(2,3,0);
        mainGraph->addArc(3,4,0);
        mainGraph->addArc(7,3,0);
        mainGraph->addArc(7,6,0);
        mainGraph->addArc(6,5,0);
        mainGraph->addArc(5,2,0);

        mainGraph->applyNextDelta();


        Algora::FastPropertyMap<unsigned long long> partitionMap;

        partitionMap[mainGraph->getCurrentVertexForId(0)] = 0;
        partitionMap[mainGraph->getCurrentVertexForId(1)]= 0;//graph1->getCurrentVertexForId(1);
        partitionMap[mainGraph->getCurrentVertexForId(2)]= 0;//graph1->getCurrentVertexForId(2);
        partitionMap[mainGraph->getCurrentVertexForId(3)]= 1;//graph2->getCurrentVertexForId(3);
        partitionMap[mainGraph->getCurrentVertexForId(4)]= 1;//graph2->getCurrentVertexForId(4);
        partitionMap[mainGraph->getCurrentVertexForId(5)]= 2;//graph3->getCurrentVertexForId(5);
        partitionMap[mainGraph->getCurrentVertexForId(6)]= 2;//graph3->getCurrentVertexForId(6);
        partitionMap[mainGraph->getCurrentVertexForId(7)]= 2;


        algorithm = new PartitionedDAPReachAlgorithmImplementation<T>();
        algorithm->setGraph(mainGraph->getDiGraph());
        algorithm->partition(partitionMap, 3);
    }
};

using TestTypes = ::testing::Types<SSBasedDAPReachAlgorithm<Algora::StaticDFSSSReachAlgorithm>, SSBasedDAPReachAlgorithm<Algora::StaticBFSSSReachAlgorithm>, SSBasedDAPReachAlgorithm<Algora::CachingBFSSSReachAlgorithm>, SSBasedDAPReachAlgorithm<Algora::CachingDFSSSReachAlgorithm>, SSBasedDAPReachAlgorithm<Algora::LazyDFSSSReachAlgorithm>, SSBasedDAPReachAlgorithm<Algora::LazyBFSSSReachAlgorithm>, SSBasedDAPReachAlgorithm<Algora::SimpleIncSSReachAlgorithm>, SSBasedDAPReachAlgorithm<Algora::ESTreeQ>, SSBasedDAPReachAlgorithm<Algora::OldESTree>, SSBasedDAPReachAlgorithm<Algora::ESTreeML>, SSBasedDAPReachAlgorithm<Algora::SimpleESTree>>;
TYPED_TEST_SUITE(TestPartitionedDAPReachAlgorithm, TestTypes);

TYPED_TEST(TestPartitionedDAPReachAlgorithm, testSameRegion){

    Algora::Vertex* vertex1 = this->mainGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex2 = this->mainGraph -> getCurrentVertexForId(2);

    ASSERT_TRUE(this->algorithm -> query(vertex1, vertex2));
}

TYPED_TEST(TestPartitionedDAPReachAlgorithm, testOverlayConnection){

    Algora::Vertex* vertex1 = this->mainGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex4 = this->mainGraph -> getCurrentVertexForId(4);

    ASSERT_TRUE(this->algorithm -> query(vertex1, vertex4));
}

TYPED_TEST(TestPartitionedDAPReachAlgorithm, testSameVertex){

    Algora::Vertex* vertex1 = this->mainGraph -> getCurrentVertexForId(1);

    ASSERT_TRUE(this->algorithm -> query(vertex1, vertex1));
}

TYPED_TEST(TestPartitionedDAPReachAlgorithm, testWrongDirection){

    Algora::Vertex* vertex1 = this->mainGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex4 = this->mainGraph -> getCurrentVertexForId(4);

    ASSERT_FALSE(this->algorithm -> query(vertex4, vertex1));
}


TYPED_TEST(TestPartitionedDAPReachAlgorithm, testDynamicSameRegion1){

    Algora::Vertex* vertex2 = this->mainGraph -> getCurrentVertexForId(2);
    Algora::Vertex* vertex1 = this->mainGraph -> getCurrentVertexForId(1);

    ASSERT_FALSE(this->algorithm -> query(vertex2, vertex1));

    this->mainGraph->addArc(2,1,1);

    this->mainGraph->applyNextDelta();

    ASSERT_TRUE(this->algorithm -> query(vertex2, vertex1));
}

TYPED_TEST(TestPartitionedDAPReachAlgorithm, testDynamicSameRegion2){

    Algora::Vertex* vertex3 = this->mainGraph -> getCurrentVertexForId(3);
    Algora::Vertex* vertex4 = this->mainGraph -> getCurrentVertexForId(4);

    ASSERT_FALSE(this->algorithm -> query(vertex4, vertex3));

    this->mainGraph->addArc(4,3,1);
    this->mainGraph->applyNextDelta();

    ASSERT_TRUE(this->algorithm -> query(vertex4, vertex3));
}

TYPED_TEST(TestPartitionedDAPReachAlgorithm, testDynamicOtherRegion){

    Algora::Vertex* vertex1 = this->mainGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex2 = this->mainGraph -> getCurrentVertexForId(2);
    Algora::Vertex* vertex3 = this->mainGraph -> getCurrentVertexForId(3);
    Algora::Vertex* vertex4 = this->mainGraph -> getCurrentVertexForId(4);

    ASSERT_FALSE(this->algorithm -> query(vertex4, vertex1));

    this->mainGraph->addArc(4,3,1);
    this->mainGraph->addArc(3,2,1);
    this->mainGraph->addArc(2,1,1);
    this->mainGraph->applyNextDelta();


    ASSERT_TRUE(this->algorithm -> query(vertex3, vertex2));
    ASSERT_TRUE(this->algorithm -> query(vertex4, vertex1));
}

TYPED_TEST(TestPartitionedDAPReachAlgorithm, testDynamicOverlayRegion){

    Algora::Vertex* vertex3 = this->mainGraph -> getCurrentVertexForId(3);
    Algora::Vertex* vertex2 = this->mainGraph -> getCurrentVertexForId(2);

    ASSERT_FALSE(this->algorithm -> query(vertex3, vertex2));

    this->mainGraph->addArc(3,2,1);
    this->mainGraph->applyNextDelta();

    ASSERT_TRUE(this->algorithm -> query(vertex3, vertex2));
}

TYPED_TEST(TestPartitionedDAPReachAlgorithm, testOverlayRegion){

    Algora::Vertex* vertex3 = this->mainGraph -> getCurrentVertexForId(3);
    Algora::Vertex* vertex2 = this->mainGraph -> getCurrentVertexForId(2);

    ASSERT_TRUE(this->algorithm -> query(vertex2, vertex3));
}

TYPED_TEST(TestPartitionedDAPReachAlgorithm, testOverlayMultipleVertices){

    Algora::Vertex* vertex6 = this->mainGraph -> getCurrentVertexForId(6);
    Algora::Vertex* vertex4 = this->mainGraph -> getCurrentVertexForId(4);

    ASSERT_TRUE(this->algorithm -> query(vertex6, vertex4));
}

TYPED_TEST(TestPartitionedDAPReachAlgorithm, testNewOverlayVertex){

    Algora::Vertex* vertex1 = this->mainGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex7 = this->mainGraph -> getCurrentVertexForId(7);

    this->mainGraph->addArc(6,1,1);
    this->mainGraph->applyNextDelta();

    ASSERT_TRUE(this->algorithm -> query(vertex7, vertex1));
}

TYPED_TEST(TestPartitionedDAPReachAlgorithm, testNewOverlayVertexRemoved){

    Algora::Vertex* vertex1 = this->mainGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex7 = this->mainGraph -> getCurrentVertexForId(7);

    this->mainGraph->addArc(6,1,1);
    this->mainGraph->applyNextDelta();

    ASSERT_TRUE(this->algorithm -> query(vertex7, vertex1));

    this->mainGraph->removeArc(6,1,2);
    this->mainGraph->applyNextDelta();

    ASSERT_FALSE(this->algorithm -> query(vertex7, vertex1));
}

TYPED_TEST(TestPartitionedDAPReachAlgorithm, testPreviousConnectionRemoved){

    Algora::Vertex* vertex1 = this->mainGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex2 = this->mainGraph -> getCurrentVertexForId(2);

    ASSERT_TRUE(this->algorithm -> query(vertex1, vertex2));

    this->mainGraph->removeArc(1,2,1);
    this->mainGraph->applyNextDelta();

    ASSERT_FALSE(this->algorithm -> query(vertex1, vertex2));
}

TYPED_TEST(TestPartitionedDAPReachAlgorithm, testRemoveOverlayArc){

    Algora::Vertex* vertex2 = this->mainGraph -> getCurrentVertexForId(2);
    Algora::Vertex* vertex3 = this->mainGraph -> getCurrentVertexForId(3);

    ASSERT_TRUE(this->algorithm -> query(vertex2, vertex3));

    this->mainGraph->removeArc(2,3,1);
    this->mainGraph->applyNextDelta();

    ASSERT_FALSE(this->algorithm -> query(vertex2, vertex3));
}

TYPED_TEST(TestPartitionedDAPReachAlgorithm, testDoubleOverlayQuery){

    Algora::Vertex* vertex2 = this->mainGraph -> getCurrentVertexForId(2);
    Algora::Vertex* vertex3 = this->mainGraph -> getCurrentVertexForId(3);

    ASSERT_TRUE(this->algorithm -> query(vertex2, vertex3));
    ASSERT_TRUE(this->algorithm -> query(vertex2, vertex3));
}

TYPED_TEST(TestPartitionedDAPReachAlgorithm, testDoubleTwoWayOverlayQuery){

    Algora::Vertex* vertex1 = this->mainGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex0 = this->mainGraph -> getCurrentVertexForId(0);
    Algora::Vertex* vertex5 = this->mainGraph -> getCurrentVertexForId(5);

    ASSERT_FALSE(this->algorithm -> query(vertex5, vertex1));
    ASSERT_TRUE(this->algorithm -> query(vertex5, vertex0));
}

