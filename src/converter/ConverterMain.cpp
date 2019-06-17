//
// Created by Alwin Stockinger.
//

#include "GraphFileConverter.h"

int main(int argc, char *argv[]){
/*
    std::string graphType = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile = "graph";
    if(argc == 4) outputFile = argv[3];

    if( graphType == "kahipDi"){
        GraphFileConverter::convertKahipDiToUnDi(inputFile, outputFile);

        void GraphFileConverter::randomToKahip(const std::string& outputFileName, const unsigned long long graphSize, const unsigned long long arcSize) {
            Algora::RandomInstanceProvider provider;
            provider.setGraphSize(graphSize);
            if( arcSizeOption){
                provider.setInitialArcSize(arcSize);
            } else{
                provider.setInitialArcProbability(arcProb);
            }
            provider.setNumOperations(operationSize);
            provider.setArcAdditionProportion(propAddition);
            provider.setArcRemovalProportion(propRemoval);
            provider.setQueriesProportion(propQuery);
            provider.setMultiplier(multiplier);
            provider.nextInstance();

            Algora::DynamicDiGraph dynGraph = provider.getGraph();
            dynGraph.resetToBigBang();
            dynGraph.applyNextDelta();

            std::map<unsigned long long int, std::map<unsigned long long int, unsigned long long int>> vertexMap = generateVertexMapFromGraph(dynGraph.getDiGraph());
            writeMapToFile(outputFileName, dynGraph.getDiGraph(), vertexMap);
        }

        void GraphFileConverter::convertKonectToKahip(const std::string& inputFileName, const std::string& outputFileName) {
            Algora::KonectNetworkInstanceProvider provider;
            provider.addInputFile(inputFileName);
            provider.nextInstance();

            Algora::DynamicDiGraph dynGraph = provider.getGraph();
            dynGraph.resetToBigBang();
            dynGraph.applyNextDelta();
        }
    }
    else if(graphType == "konect"){
        GraphFileConverter::convertKonectToKahip(inputFile, outputFile);
    }
    else if(graphType == "random"){
        GraphFileConverter::randomToKahip(outputFile);
    }
    else {
        std::cerr << "graphType " + graphType + " not supported";
        return 1;
    }*/


    return 0;
}