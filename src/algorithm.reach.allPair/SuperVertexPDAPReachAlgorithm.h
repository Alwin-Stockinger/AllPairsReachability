//
// Created by alwin on 7/22/19.
//

#ifndef ALLPAIRREACH_SUPERVERTEXPDAPREACHALGORITHM_H
#define ALLPAIRREACH_SUPERVERTEXPDAPREACHALGORITHM_H


#include "PartitionedDAPReachAlgorithm.h"
#include "PartitionedDAPReachAlgorithmImplementation.h"

// PartitionedDAPReachAlgorithmImplementation<SubAlgorithm,OverlayAlgorithm, propagatePartitionFunction >
template <typename SubAlgorithm, typename OverlayAlgorithm = SubAlgorithm, bool propagatePartitionFunction = true>
class SuperVertexPDAPReachAlgorithm : public PartitionedDAPReachAlgorithmImplementation<SubAlgorithm,OverlayAlgorithm, propagatePartitionFunction >{

public:

    bool query(Algora::Vertex *start, const Algora::Vertex *end) override {

        if (!this->initialized) {
            this->run();
        }

        start = this->mainToSubMap[start];
        end = this->mainToSubMap[end];

        //select subgraphs
        auto *startGraph = start->getParent();
        auto *endGraph = end->getParent();

        //same subgraph? then normal Algorithm
        if (startGraph == endGraph) {
            return this->graphToAlgorithmMap[startGraph]->query(start, end);
        } else {
            DynamicAPReachAlgorithm *startGraphAlgorithm = this->graphToAlgorithmMap[startGraph];
            DynamicAPReachAlgorithm *endGraphAlgorithm = this->graphToAlgorithmMap[endGraph];

            std::set<Algora::Vertex *> &startEdgeVertices = this->edgeVertices[startGraph];
            std::set<Algora::Vertex *> &endEdgeVertices = this->edgeVertices[endGraph];

            Algora::Vertex *sourceSuperVertex = this->overlayGraph->addVertex();
            Algora::Vertex *destinationSuperVertex = this->overlayGraph->addVertex();

            //build outgoing super vertex
            for (Algora::Vertex *outVertex : startEdgeVertices) {
                if (startGraphAlgorithm->query(start, this->mainToSubMap[outVertex])) {
                    this->overlayGraph->mapOutgoingArcs(this->mainToOverlayMap[outVertex],
                                                  [this, sourceSuperVertex](Algora::Arc *arc) {
                                                      this->overlayGraph->addArc(sourceSuperVertex, arc->getHead());
                                                  });
                }
            }

            //build incoming super vertex
            for (Algora::Vertex *inVertex : endEdgeVertices) {
                if (endGraphAlgorithm->query(this->mainToSubMap[inVertex], end)) {
                    this->overlayGraph->mapIncomingArcs(this->mainToOverlayMap[inVertex],
                                                  [this, destinationSuperVertex](Algora::Arc *arc) {
                                                      this->overlayGraph->addArc(arc->getTail(),
                                                                                 destinationSuperVertex);
                                                  });
                }
            }

            bool result = this->overlayAlgorithm->query(sourceSuperVertex, destinationSuperVertex);

            this->overlayGraph->removeVertex(sourceSuperVertex);
            this->overlayGraph->removeVertex(destinationSuperVertex);

            return result;
        }
    }
};


#endif //ALLPAIRREACH_SUPERVERTEXPDAPREACHALGORITHM_H
