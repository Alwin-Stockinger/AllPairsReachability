//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_DFSOFACTORY_H
#define ALLPAIRREACH_DFSOFACTORY_H


#include "PartitionedFactory.h"

class DFSOFactory : public PartitionedFactory{

    std::vector<DynamicAPReachAlgorithm *> getStaticBFSAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getStaticDFSAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getCachingBFSAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getCachingDFSAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getLazyBFSAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getLazyDFSAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getSimpleISSRAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getSimpleESTAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getMLESTAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getESTAlgorithms() override;

    std::vector<DynamicAPReachAlgorithm *> getOldESTAlgorithms() override;

private:

    template <typename SubAlgorithm>
    std::vector<DynamicAPReachAlgorithm*> createAlgorithms();

public:
    void setAdvancedDFSO(bool b);

    void setSimpleDFSO(bool b);

    bool advancedOverlaySearch = false;
    bool simpleOverlaySearch = false;
};


#endif //ALLPAIRREACH_DFSOFACTORY_H