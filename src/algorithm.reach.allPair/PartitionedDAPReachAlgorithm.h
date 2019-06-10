//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H
#define ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H

#include "property/fastpropertymap.h"

#include "DynamicAPReachAlgorithm.h"

class PartitionedDAPReachAlgorithm : public DynamicAPReachAlgorithm{
public:
    virtual void partition(const Algora::FastPropertyMap<unsigned long long int> &partitionMap,
                   unsigned long long int k) = 0;

};


#endif //ALLPAIRREACH_PARTITIONEDDAPREACHALGORITHM_H
