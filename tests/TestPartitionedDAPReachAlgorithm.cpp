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

public:
    PartitionedDAPReachAlgorithm<SSBasedDAPReachAlgorithm<Algora::StaticBFSSSReachAlgorithm>>* algorithm;

    Algora::DynamicDiGraph* mainGraph;
    std::vector<Algora::SubDiGraph*> graphs;
    Algora::SubDiGraph* overlayGraph;
    std::vector<Algora::Property<bool>*> properties;

    ~TestPartitionedDAPReachAlgorithm() override{
        delete algorithm;


        delete overlayGraph;

        for( Algora::SubDiGraph* graph: graphs){
            delete graph;
        }
        for( Algora::Property<bool>* property: properties){
            delete property;
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

        mainGraph->addArc(1,2,0);
        mainGraph->addArc(2,3,0);
        mainGraph->addArc(3,4,0);


        mainGraph->applyNextDelta();

        Algora::DiGraph* diGraph = mainGraph->getDiGraph();



        Algora::FastPropertyMap<bool>* map1 = new Algora::FastPropertyMap<bool>;
        map1->setValue(mainGraph->getCurrentVertexForId(1), true);
        map1->setValue(mainGraph->getCurrentVertexForId(2), true);
        auto* graph1 = new Algora::SubDiGraph(diGraph, *map1);
        graphs.push_back(graph1);
        properties.push_back(map1);

        Algora::FastPropertyMap<bool>* map2= new Algora::FastPropertyMap<bool>;
        map2->setValue(mainGraph->getCurrentVertexForId(3), true);
        map2->setValue(mainGraph->getCurrentVertexForId(4), true);
        auto* graph2 = new Algora::SubDiGraph(diGraph, *map2);
        graphs.push_back(graph2);
        properties.push_back(map2);


        Algora::FastPropertyMap<bool>* overlayMap = new Algora::FastPropertyMap<bool>;
        overlayMap->setValue(mainGraph->getCurrentVertexForId(2), true);
        overlayMap->setValue(mainGraph->getCurrentVertexForId(3), true);
        overlayGraph = new Algora::SubDiGraph(diGraph, *overlayMap);
        properties.push_back(overlayMap);

        algorithm = new PartitionedDAPReachAlgorithm<SSBasedDAPReachAlgorithm<Algora::StaticBFSSSReachAlgorithm>>({graph1, graph2}, overlayGraph);
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


