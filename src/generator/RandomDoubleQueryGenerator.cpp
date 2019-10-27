//
// Created by Alwin Stockinger.
//

#include "RandomDoubleQueryGenerator.h"

std::vector<Algora::DynamicDiGraphQueryProvider::VertexQueryList>
RandomDoubleQueryGenerator::provideVertexQueries(Algora::DynamicDiGraph *dyGraph) {
    return RandomQueryGenerator::provideVertexQueries(dyGraph);
}

std::string RandomDoubleQueryGenerator::getName()  const noexcept {
    return "Random Double Query Generator";
}
