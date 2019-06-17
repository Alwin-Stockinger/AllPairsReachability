//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHMIMPLEMENTATION_H
#define ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHMIMPLEMENTATION_H

#include <map>
#include <set>
#include <property/propertymap.h>

#include "DynamicAPReachAlgorithm.h"
#include "SSBasedDAPReachAlgorithm.h"
#include "PartitionedDAPReachAlgorithm.h"

template <typename T>
class PartitionedDAPReachAlgorithmImplementation : public PartitionedDAPReachAlgorithm{

public:
    static_assert(std::is_base_of<DynamicAPReachAlgorithm,T>::value, "Template Parameter has to inherit from DynamicApAlgorithm");

    explicit PartitionedDAPReachAlgorithmImplementation() : PartitionedDAPReachAlgorithm(){};

    ~PartitionedDAPReachAlgorithmImplementation() override = default;

protected:
    DynamicAPReachAlgorithm *createSubAlgorithm() override {
        return new T();
    }
};


#endif //ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H
