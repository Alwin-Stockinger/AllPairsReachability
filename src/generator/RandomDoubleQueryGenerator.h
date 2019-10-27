//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_RANDOMDOUBLEQUERYGENERATOR_H
#define ALLPAIRREACH_RANDOMDOUBLEQUERYGENERATOR_H

#include "graph.dyn.generator/randomquerygenerator.h"

class RandomDoubleQueryGenerator: Algora::RandomQueryGenerator{

    std::vector<VertexQueryList> provideVertexQueries(Algora::DynamicDiGraph *dyGraph) override;

    std::string getName() const noexcept override;

};


#endif //ALLPAIRREACH_RANDOMDOUBLEQUERYGENERATOR_H
