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