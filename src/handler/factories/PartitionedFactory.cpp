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