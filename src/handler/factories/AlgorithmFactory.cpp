//
// Created by Alwin Stockinger.
//

#include "AlgorithmFactory.h"

std::vector<DynamicAPReachAlgorithm *> AlgorithmFactory::getAlgorithms() {
    std::vector<DynamicAPReachAlgorithm*> algorithms;


    for(const std::string& algorithmName : algorithmNames){

        std::vector<DynamicAPReachAlgorithm*> newAlgorithms;

        if(algorithmName == "StaticBFS") {
            newAlgorithms = getStaticBFSAlgorithms();
        }
        else if(algorithmName == "StaticDFS") {
            newAlgorithms = getStaticDFSAlgorithms();
        }
        else if( algorithmName == "LazyDFS") {
            newAlgorithms = getLazyDFSAlgorithms();
        }
        else if( algorithmName == "LazyBFS") {
            newAlgorithms = getLazyBFSAlgorithms();
        }
        else if( algorithmName == "CachingDFS") {
            newAlgorithms = getCachingDFSAlgorithms();
        }
        else if( algorithmName == "CachingBFS") {
            newAlgorithms = getCachingBFSAlgorithms();
        }
        else if( algorithmName == "SimpleInc") {
            newAlgorithms = getSimpleISSRAlgorithms();
        }
        else if( algorithmName == "ESTreeML") {
            newAlgorithms = getMLESTAlgorithms();
        }
        else if( algorithmName == "OldESTree") {
            newAlgorithms = getOldESTAlgorithms();
        }
        else if( algorithmName == "ESTreeQ") {
            newAlgorithms = getESTAlgorithms();
        }
        else if( algorithmName == "SimpleESTree") {
            newAlgorithms = getSimpleESTAlgorithms();
        }
        else{
            throw std::invalid_argument(algorithmName + " is not a viable algorithm");
        }

        algorithms.insert(algorithms.end(), newAlgorithms.begin(), newAlgorithms.end());
    }
    return algorithms;
}
