//
// Created by Alwin Stockinger.
//

#include "../src/algorithm.reach.allPair/PartitionedDAPReachAlgorithm.h"
#include "../src/algorithm.reach.allPair/SSBasedDAPReachAlgorithm.h"

#include <gtest/gtest.h>

#include <algorithm.reach/staticbfsssreachalgorithm.h>
#include <graph.dyn/dynamicdigraph.h>
#include <graph/subdigraph.h>


class TestPartitionedDAPReachAlgorithm : public testing::Test{

protected:
    PartitionedDAPReachAlgorithm<SSBasedDAPReachAlgorithm<Algora::StaticBFSSSReachAlgorithm>>* algorithm;

    Algora::DynamicDiGraph* mainGraph;
    std::vector<Algora::DynamicDiGraph*> graphs;
    Algora::DynamicDiGraph* overlayGraph;

    std::vector<Algora::DiGraph*> subGraphs;

    ~TestPartitionedDAPReachAlgorithm() override{
        delete algorithm;

        delete overlayGraph;

        for( Algora::DynamicDiGraph* graph: graphs){
            delete graph;
        }

        delete mainGraph;
    }

public:
    void TearDown() override {
    }

    void SetUp() override {

        mainGraph = new Algora::DynamicDiGraph;

        mainGraph->addVertex(1, 0);
        mainGraph->addVertex(2, 0);
        mainGraph->addVertex(3, 0);
        mainGraph->addVertex(4, 0);
        mainGraph->addVertex(5, 0);
        mainGraph->addVertex(6, 0);
        mainGraph->addVertex(7, 0);
        mainGraph->addArc(1,2,0);
        mainGraph->addArc(2,3,0);
        mainGraph->addArc(3,4,0);
        mainGraph->addArc(7,3,0);
        mainGraph->addArc(7,6,0);
        mainGraph->addArc(6,5,0);
        mainGraph->addArc(5,2,0);

        mainGraph->applyNextDelta();


        auto* graph1 = new Algora::DynamicDiGraph;
        graph1->addVertex(1,0);
        graph1->addVertex(2,0);
        graph1->addArc(1,2,0);
        graph1->applyNextDelta();
        graphs.push_back(graph1);
        subGraphs.push_back(graph1->getDiGraph());

        auto* graph2 = new Algora::DynamicDiGraph;
        graph2->addVertex(3,0);
        graph2->addVertex(4,0);
        graph2->addArc(3,4,0);
        graph2->applyNextDelta();
        graphs.push_back(graph2);
        subGraphs.push_back(graph2->getDiGraph());

        auto* graph3 = new Algora::DynamicDiGraph;
        graph3->addVertex(5,0);
        graph3->addVertex(6,0);
        graph3->addVertex(7,0);
        graph3->addArc(6,5,0);
        graph3->addArc(7,6,0);
        graph3->applyNextDelta();
        graphs.push_back(graph3);
        subGraphs.push_back(graph3->getDiGraph());

        overlayGraph = new Algora::DynamicDiGraph;
        overlayGraph->addVertex(1, 0);
        overlayGraph->addVertex(2, 0);
        overlayGraph->addVertex(3, 0);
        overlayGraph->addVertex(4, 0);
        overlayGraph->addVertex(5, 0);
        overlayGraph->addVertex(6, 0);
        overlayGraph->addVertex(7, 0);
        overlayGraph->addArc(2,3,0);
        overlayGraph->addArc(5,2,0);
        overlayGraph->addArc(7,3,0);
        overlayGraph->applyNextDelta();


        std::map<const Algora::Vertex*, Algora::Vertex*> inMap;

        inMap[overlayGraph->getCurrentVertexForId(1)]= graph1->getCurrentVertexForId(1);
        inMap[overlayGraph->getCurrentVertexForId(2)]= graph1->getCurrentVertexForId(2);
        inMap[overlayGraph->getCurrentVertexForId(3)]= graph2->getCurrentVertexForId(3);
        inMap[overlayGraph->getCurrentVertexForId(4)]= graph2->getCurrentVertexForId(4);
        inMap[overlayGraph->getCurrentVertexForId(5)]= graph3->getCurrentVertexForId(5);
        inMap[overlayGraph->getCurrentVertexForId(6)]= graph3->getCurrentVertexForId(6);
        inMap[overlayGraph->getCurrentVertexForId(7)]= graph3->getCurrentVertexForId(7);

        std::map<const Algora::Vertex*, Algora::Vertex*> mainToOverlayMap;

        for(int i=1; i<8; i++){
            mainToOverlayMap[mainGraph->getCurrentVertexForId(i)] = overlayGraph->getCurrentVertexForId(i);
        }


        algorithm = new PartitionedDAPReachAlgorithm<SSBasedDAPReachAlgorithm<Algora::StaticBFSSSReachAlgorithm>>();
        algorithm->setGraph(mainGraph->getDiGraph());
        algorithm->setGraphs(
                &subGraphs, overlayGraph->getDiGraph(), inMap, mainToOverlayMap);
    }
};

TEST_F(TestPartitionedDAPReachAlgorithm, testSameRegion){

    Algora::Vertex* vertex1 = mainGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex2 = mainGraph -> getCurrentVertexForId(2);

    ASSERT_TRUE(algorithm -> query(vertex1, vertex2));
}

TEST_F(TestPartitionedDAPReachAlgorithm, testOverlayConnection){

    Algora::Vertex* vertex1 = mainGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex4 = mainGraph -> getCurrentVertexForId(4);

    ASSERT_TRUE(algorithm -> query(vertex1, vertex4));
}

TEST_F(TestPartitionedDAPReachAlgorithm, testSameVertex){

    Algora::Vertex* vertex1 = mainGraph -> getCurrentVertexForId(1);

    ASSERT_TRUE(algorithm -> query(vertex1, vertex1));
}

TEST_F(TestPartitionedDAPReachAlgorithm, testWrongDirection){

    Algora::Vertex* vertex1 = mainGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex4 = mainGraph -> getCurrentVertexForId(4);

    ASSERT_FALSE(algorithm -> query(vertex4, vertex1));
}


TEST_F(TestPartitionedDAPReachAlgorithm, testDynamicSameRegion1){

    Algora::Vertex* vertex2 = mainGraph -> getCurrentVertexForId(2);
    Algora::Vertex* vertex1 = mainGraph -> getCurrentVertexForId(1);

    ASSERT_FALSE(algorithm -> query(vertex2, vertex1));

    mainGraph->addArc(2,1,1);

    mainGraph->applyNextDelta();

    ASSERT_TRUE(algorithm -> query(vertex2, vertex1));
}

TEST_F(TestPartitionedDAPReachAlgorithm, testDynamicSameRegion2){

    Algora::Vertex* vertex3 = mainGraph -> getCurrentVertexForId(3);
    Algora::Vertex* vertex4 = mainGraph -> getCurrentVertexForId(4);

    ASSERT_FALSE(algorithm -> query(vertex4, vertex3));

    mainGraph->addArc(4,3,1);
    mainGraph->applyNextDelta();

    ASSERT_TRUE(algorithm -> query(vertex4, vertex3));
}

TEST_F(TestPartitionedDAPReachAlgorithm, testDynamicOtherRegion){

    Algora::Vertex* vertex1 = mainGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex2 = mainGraph -> getCurrentVertexForId(2);
    Algora::Vertex* vertex3 = mainGraph -> getCurrentVertexForId(3);
    Algora::Vertex* vertex4 = mainGraph -> getCurrentVertexForId(4);

    ASSERT_FALSE(algorithm -> query(vertex4, vertex1));

    mainGraph->addArc(4,3,1);
    mainGraph->addArc(3,2,1);
    mainGraph->addArc(2,1,1);
    mainGraph->applyNextDelta();


    //ASSERT_TRUE(algorithm -> query(vertex2, vertex1));
    ASSERT_TRUE(algorithm -> query(vertex3, vertex2));
    //ASSERT_TRUE(algorithm -> query(vertex4, vertex3));
    ASSERT_TRUE(algorithm -> query(vertex4, vertex1));
}

TEST_F(TestPartitionedDAPReachAlgorithm, testDynamicOverlayRegion){

    Algora::Vertex* vertex3 = mainGraph -> getCurrentVertexForId(3);
    Algora::Vertex* vertex2 = mainGraph -> getCurrentVertexForId(2);

    ASSERT_FALSE(algorithm -> query(vertex3, vertex2));

    mainGraph->addArc(3,2,1);
    mainGraph->applyNextDelta();

    ASSERT_TRUE(algorithm -> query(vertex3, vertex2));
}

TEST_F(TestPartitionedDAPReachAlgorithm, testOverlayRegion){

    Algora::Vertex* vertex3 = mainGraph -> getCurrentVertexForId(3);
    Algora::Vertex* vertex2 = mainGraph -> getCurrentVertexForId(2);

    ASSERT_TRUE(algorithm -> query(vertex2, vertex3));
}

TEST_F(TestPartitionedDAPReachAlgorithm, testOverlayMultipleVertices){

    Algora::Vertex* vertex6 = mainGraph -> getCurrentVertexForId(6);
    Algora::Vertex* vertex4 = mainGraph -> getCurrentVertexForId(4);

    ASSERT_TRUE(algorithm -> query(vertex6, vertex4));
}

TEST_F(TestPartitionedDAPReachAlgorithm, testNewOverlayVertex){

    Algora::Vertex* vertex1 = mainGraph -> getCurrentVertexForId(1);
    Algora::Vertex* vertex7 = mainGraph -> getCurrentVertexForId(7);

    mainGraph->addArc(6,1,1);
    mainGraph->applyNextDelta();

    ASSERT_TRUE(algorithm -> query(vertex7, vertex1));
}

