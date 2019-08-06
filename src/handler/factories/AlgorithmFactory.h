//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_ALGORITHMFACTORY_H
#define ALLPAIRREACH_ALGORITHMFACTORY_H


#include <utility>

#include "../../algorithm.reach.allPair/DynamicAPReachAlgorithm.h"

class AlgorithmFactory {
public:

    void setAlgorithmNames(std::vector<std::string> newAlgorithmNames){
        algorithmNames = std::move(newAlgorithmNames);
    }

    virtual std::vector<DynamicAPReachAlgorithm*> getAlgorithms();

protected:

    virtual std::vector <DynamicAPReachAlgorithm*> getStaticBFSAlgorithms() = 0;
    virtual std::vector <DynamicAPReachAlgorithm*> getStaticDFSAlgorithms() = 0;
    virtual std::vector <DynamicAPReachAlgorithm*> getCachingBFSAlgorithms() = 0;
    virtual std::vector <DynamicAPReachAlgorithm*> getCachingDFSAlgorithms() = 0;
    virtual std::vector <DynamicAPReachAlgorithm*> getLazyBFSAlgorithms() = 0;
    virtual std::vector <DynamicAPReachAlgorithm*> getLazyDFSAlgorithms() = 0;
    virtual std::vector <DynamicAPReachAlgorithm*> getSimpleISSRAlgorithms() = 0;
    virtual std::vector <DynamicAPReachAlgorithm*> getSimpleESTAlgorithms() = 0;
    virtual std::vector <DynamicAPReachAlgorithm*> getMLESTAlgorithms() = 0;
    virtual std::vector <DynamicAPReachAlgorithm*> getESTAlgorithms() = 0;
    virtual std::vector <DynamicAPReachAlgorithm*> getOldESTAlgorithms() = 0;

protected:
    std::vector<std::string> algorithmNames;


};


#endif //ALLPAIRREACH_ALGORITHMFACTORY_H
