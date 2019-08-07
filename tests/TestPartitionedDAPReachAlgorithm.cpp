//
// Created by Alwin Stockinger.
//

#include "../src/algorithm.reach.allPair/SimplePartitionedDAPReachAlgorithmImplementation.h"
#include "../src/algorithm.reach.allPair/SSBasedDAPReachAlgorithmImplementation.h"
#include "../src/algorithm.reach.allPair/SuperVertexAlgorithmImplementation.h"
#include "../src/algorithm.reach.allPair/BFSOPDAPReachAlgorithmImplementation.h"
#include "../src/algorithm.reach.allPair/ReverseBFSPartitionedAPReachAlgorithm.h"

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
#include <graph.incidencelist/incidencelistgraph.h>


template <typename T>
class TestPartitionedDAPReachAlgorithm : public testing::Test{

protected:
    PartitionedAPReachAlgorithm* algorithm = nullptr;

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
        mainGraph->addVertex(8, 0);
        mainGraph->addArc(2,0,0);
        mainGraph->addArc(1,2,0);
        mainGraph->addArc(2,3,0);
        mainGraph->addArc(3,4,0);
        mainGraph->addArc(7,3,0);
        mainGraph->addArc(7,6,0);
        mainGraph->addArc(6,5,0);
        mainGraph->addArc(5,2,0);
        mainGraph->addArc(4, 8, 0);

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
        partitionMap[mainGraph->getCurrentVertexForId(8)] = 3;


        algorithm = new T();


        /*Algora::FastPropertyMap<unsigned long long> (*partitionFunction) (unsigned long long int, Algora::DiGraph*) = [partitionMap] (unsigned long long k, Algora::DiGraph* diGraph){
            return partitionMap;
        };*/


        algorithm->setPartitionFunction([partitionMap] (unsigned long long k, Algora::DiGraph* diGraph){
            return partitionMap;
        }, 4);

        algorithm->setGraph(mainGraph->getDiGraph());

        algorithm->run();
    }
};


using TestTypes = ::testing::Types<
        SimplePartitionedDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::StaticDFSSSReachAlgorithm>>, SimplePartitionedDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::StaticBFSSSReachAlgorithm>>, SimplePartitionedDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::CachingBFSSSReachAlgorithm>>, SimplePartitionedDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::CachingDFSSSReachAlgorithm>>, SimplePartitionedDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::LazyDFSSSReachAlgorithm>>, SimplePartitionedDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::LazyBFSSSReachAlgorithm>>, SimplePartitionedDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::SimpleIncSSReachAlgorithm>>, SimplePartitionedDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::ESTreeQ>>, SimplePartitionedDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::ESTreeML>>, SimplePartitionedDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::SimpleESTree>>,
        SuperVertexAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::StaticDFSSSReachAlgorithm>,Algora::StaticDFSSSReachAlgorithm>, SuperVertexAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::StaticBFSSSReachAlgorithm>,Algora::StaticBFSSSReachAlgorithm>, SuperVertexAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::CachingBFSSSReachAlgorithm>,Algora::CachingBFSSSReachAlgorithm>, SuperVertexAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::CachingDFSSSReachAlgorithm>,Algora::CachingDFSSSReachAlgorithm>, SuperVertexAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::LazyDFSSSReachAlgorithm>,Algora::LazyDFSSSReachAlgorithm>, SuperVertexAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::LazyBFSSSReachAlgorithm>,Algora::LazyBFSSSReachAlgorithm>, SuperVertexAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::SimpleIncSSReachAlgorithm>,Algora::SimpleIncSSReachAlgorithm>, SuperVertexAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::ESTreeQ>,Algora::ESTreeQ>, SuperVertexAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::ESTreeML>,Algora::ESTreeML>, SuperVertexAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::SimpleESTree>,Algora::SimpleESTree>,
        BFSOPDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::StaticDFSSSReachAlgorithm>>, BFSOPDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::StaticBFSSSReachAlgorithm>>, BFSOPDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::CachingBFSSSReachAlgorithm>>, BFSOPDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::CachingDFSSSReachAlgorithm>>, BFSOPDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::LazyDFSSSReachAlgorithm>>, BFSOPDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::LazyBFSSSReachAlgorithm>>, BFSOPDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::SimpleIncSSReachAlgorithm>>, BFSOPDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::ESTreeQ>>, BFSOPDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::ESTreeML>>, BFSOPDAPReachAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<Algora::SimpleESTree>>,
        ReverseBFSPartitionedAPReachAlgorithm>;
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

TYPED_TEST(TestPartitionedDAPReachAlgorithm, testVertexAdditionWithArc){

    this->mainGraph->addVertex(10, 1);
    this->mainGraph->applyNextDelta();

    this->mainGraph->addArc(4, 10, 2);
    this->mainGraph->applyNextDelta();

    Algora::Vertex* vertex4 = this->mainGraph -> getCurrentVertexForId(4);
    Algora::Vertex* vertex10 = this->mainGraph -> getCurrentVertexForId(10);

    ASSERT_TRUE(this->algorithm -> query(vertex4, vertex10));
}


TYPED_TEST(TestPartitionedDAPReachAlgorithm, testVertexRemoval){

    this->mainGraph->removeVertex(3, 1);
    this->mainGraph->applyNextDelta();

    Algora::Vertex* vertex1 = this->mainGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex4 = this->mainGraph -> getCurrentVertexForId(4);

    ASSERT_FALSE(this->algorithm -> query(vertex1, vertex4));
}

TYPED_TEST(TestPartitionedDAPReachAlgorithm, testAlternativeNewOverlayVertex){

    Algora::Vertex* vertex7 = this->mainGraph -> getCurrentVertexForId(7);
    Algora::Vertex* vertex0 = this->mainGraph -> getCurrentVertexForId(0);

    Algora::Vertex* vertex5 = this->mainGraph->getCurrentVertexForId(5);
    ASSERT_FALSE(!vertex5);


    this->mainGraph->removeVertex(5,1);
    this->mainGraph->addVertex(10, 1);
    this->mainGraph->applyNextDelta();

    ASSERT_FALSE(this->algorithm->query(vertex7, vertex0));

    this->mainGraph->addArc(6, 10, 2);
    this->mainGraph->addArc(10, 2, 2);
    this->mainGraph->applyNextDelta();

    Algora::Vertex* vertex10 = this->mainGraph->getCurrentVertexForId(10);

    ASSERT_TRUE(this->algorithm -> query(vertex10, vertex0));
    ASSERT_TRUE(this->algorithm -> query(vertex7, vertex0));
}

TYPED_TEST(TestPartitionedDAPReachAlgorithm, testNewGraph){

    auto newGraph = Algora::IncidenceListGraph();

    auto* vertex1 = newGraph.addVertex();
    auto* vertex2 = newGraph.addVertex();

    this->algorithm->setPartitionFunction([vertex1, vertex2] (unsigned long long k, Algora::DiGraph* diGraph){

        Algora::FastPropertyMap<unsigned long long> map;
        map[vertex1] = 0;
        map[vertex2] = 1;

        return map;
    }, 2);

    this->algorithm->setGraph(&newGraph);



    ASSERT_FALSE(this->algorithm->query(vertex1, vertex2));

    newGraph.addArc(vertex1, vertex2);

    ASSERT_TRUE(this->algorithm -> query(vertex1, vertex2));

    this->algorithm->unsetGraph();
}

TYPED_TEST(TestPartitionedDAPReachAlgorithm, testConnectionViaOtherSubGraph){

    auto* vertex7 = this->mainGraph->getCurrentVertexForId(7);

    auto* vertex8 = this->mainGraph->getCurrentVertexForId(8);

    ASSERT_TRUE(this->algorithm -> query(vertex7, vertex8));
}

TYPED_TEST(TestPartitionedDAPReachAlgorithm, testConnectionViaOtherSubGraphRemovedThenAdded){

    auto* vertex7 = this->mainGraph->getCurrentVertexForId(7);

    auto* vertex8 = this->mainGraph->getCurrentVertexForId(8);

    ASSERT_TRUE(this->algorithm -> query(vertex7, vertex8));

    this->mainGraph->removeArc(3, 4, 1);
    this->mainGraph->applyNextDelta();

    ASSERT_FALSE(this->algorithm->query(vertex7, vertex8));

    this->mainGraph->addArc(3, 4, 2);
    this->mainGraph->applyNextDelta();

    ASSERT_TRUE(this->algorithm -> query(vertex7, vertex8));
}

TYPED_TEST(TestPartitionedDAPReachAlgorithm, testConnectionViaOtherSubGraphRemovedThenDoubleAddedAndRemoved){

    auto* vertex7 = this->mainGraph->getCurrentVertexForId(7);

    auto* vertex8 = this->mainGraph->getCurrentVertexForId(8);

    ASSERT_TRUE(this->algorithm -> query(vertex7, vertex8));

    this->mainGraph->removeArc(3, 4, 1);
    this->mainGraph->applyNextDelta();

    ASSERT_FALSE(this->algorithm->query(vertex7, vertex8));

    this->mainGraph->addArc(3, 4, 2);
    this->mainGraph->addArc(3, 4, 2);
    this->mainGraph->applyNextDelta();

    ASSERT_TRUE(this->algorithm -> query(vertex7, vertex8));


    this->mainGraph->removeArc(3, 4, 3);
    this->mainGraph->applyNextDelta();

    ASSERT_TRUE(this->algorithm -> query(vertex7, vertex8));

}

TYPED_TEST(TestPartitionedDAPReachAlgorithm, testSuperVertexESProblem){

    auto* vertex7 = this->mainGraph->getCurrentVertexForId(7);
    auto* vertex1 = this->mainGraph->getCurrentVertexForId(1);

    this->mainGraph->addArc(3,0,1);
    this->mainGraph->addArc(7,0,1);
    this->mainGraph->applyNextDelta();

    ASSERT_FALSE(this->algorithm->query(vertex7, vertex1));

    auto* vertex2 = this->mainGraph->getCurrentVertexForId(2);
    auto* vertex3 = this->mainGraph->getCurrentVertexForId(3);


    ASSERT_FALSE(this->algorithm->query(vertex3, vertex2));
}

TYPED_TEST(TestPartitionedDAPReachAlgorithm, testOverlayVertexOnSecondRemoved){

    auto* vertex2 = this->mainGraph->getCurrentVertexForId(2);
    auto* vertex3 = this->mainGraph->getCurrentVertexForId(3);
    auto* vertex7 = this->mainGraph->getCurrentVertexForId(7);

    this->mainGraph->removeArc(2,3,1);
    this->mainGraph->applyNextDelta();

    ASSERT_FALSE(this->algorithm->query(vertex2, vertex3));
    ASSERT_TRUE(this->algorithm->query(vertex7, vertex3));

    this->mainGraph->removeArc(7,3,2);
    this->mainGraph->applyNextDelta();

    ASSERT_FALSE(this->algorithm->query(vertex2, vertex3));
    ASSERT_FALSE(this->algorithm->query(vertex7, vertex3));
}

TYPED_TEST(TestPartitionedDAPReachAlgorithm, testOverlayConnectionOfSameSubgraph){
    auto* vertex1 = this->mainGraph->getCurrentVertexForId(1);
    auto* vertex2 = this->mainGraph->getCurrentVertexForId(2);
    auto* vertex3 = this->mainGraph->getCurrentVertexForId(3);
    auto* vertex4 = this->mainGraph->getCurrentVertexForId(4);

    ASSERT_FALSE(this->algorithm->query(vertex2, vertex1));

    this->mainGraph->addArc(4,1,1);
    this->mainGraph->applyNextDelta();

    ASSERT_TRUE(this->algorithm->query(vertex2, vertex1));
}