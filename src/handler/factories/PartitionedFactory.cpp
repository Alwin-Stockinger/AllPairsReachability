//
// Created by Alwin Stockinger.
//

#include "PartitionedFactory.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
void PartitionedFactory::setOverlayNames(const std::vector<std::string> &overlayNames) {
    PartitionedFactory::overlayNames = overlayNames;
}

void PartitionedFactory::setKMin(unsigned long long int kMin) {
    PartitionedFactory::kMin = kMin;
}

void PartitionedFactory::setKMax(unsigned long long int kMax) {
    PartitionedFactory::kMax = kMax;
}

void PartitionedFactory::setDepthMin(unsigned long long int depthMin) {
    PartitionedFactory::depthMin = depthMin;
}

void PartitionedFactory::setDepthMax(unsigned long long int depthMax) {
    PartitionedFactory::depthMax = depthMax;
}

void PartitionedFactory::setPartitionFunction(const PartFunc &partitionFunction) {
    PartitionedFactory::partitionFunction = partitionFunction;
}

void PartitionedFactory::setExponentialK(bool exponentialK) {
    PartitionedFactory::exponentialK = exponentialK;
}

void PartitionedFactory::setRepartitionThreshold(unsigned long long int threshold) {
    PartitionedFactory::repartitionThreshold = threshold;

}

#pragma clang diagnostic pop