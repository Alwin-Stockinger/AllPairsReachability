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

#include <algorithm.reach/cachingbfsssreachalgorithm.h>
#include <algorithm.reach/cachingdfsssreachalgorithm.h>
#include <algorithm.reach/lazybfsssreachalgorithm.h>
#include <algorithm.reach/lazydfsssreachalgorithm.h>
#include <algorithm.reach/simpleincssreachalgorithm.h>
#include <algorithm.reach.es/simpleestree.h>
#include <algorithm.reach.es/estree-ml.h>
#include <datastructure/bucketqueue.h>
#include <algorithm.reach.es/estree-queue.h>
#include <algorithm.reach.es/estree-bqueue.h>
#include "SSBasedFactory.h"
#include "../../algorithm.reach.allPair/SSBasedDAPReachAlgorithmImplementation.h"

std::vector<DynamicAPReachAlgorithm *> SSBasedFactory::getStaticBFSAlgorithms() {
    return {new SSBasedDAPReachAlgorithmImplementation<Algora::StaticBFSSSReachAlgorithm>};
}

std::vector<DynamicAPReachAlgorithm *> SSBasedFactory::getStaticDFSAlgorithms() {
    return {new SSBasedDAPReachAlgorithmImplementation<Algora::StaticDFSSSReachAlgorithm>};
}

std::vector<DynamicAPReachAlgorithm *> SSBasedFactory::getCachingBFSAlgorithms() {
    return {new SSBasedDAPReachAlgorithmImplementation<Algora::CachingBFSSSReachAlgorithm, true>};
}

std::vector<DynamicAPReachAlgorithm *> SSBasedFactory::getCachingDFSAlgorithms() {
    return {new SSBasedDAPReachAlgorithmImplementation<Algora::CachingDFSSSReachAlgorithm, true>};
}

std::vector<DynamicAPReachAlgorithm *> SSBasedFactory::getLazyBFSAlgorithms() {
    return {new SSBasedDAPReachAlgorithmImplementation<Algora::LazyBFSSSReachAlgorithm, true>};
}

std::vector<DynamicAPReachAlgorithm *> SSBasedFactory::getLazyDFSAlgorithms() {
    return {new SSBasedDAPReachAlgorithmImplementation<Algora::LazyDFSSSReachAlgorithm, true>};
}

std::vector<DynamicAPReachAlgorithm *> SSBasedFactory::getSimpleISSRAlgorithms() {
    return {new SSBasedDAPReachAlgorithmImplementation<Algora::SimpleIncSSReachAlgorithm, true>};
}

std::vector<DynamicAPReachAlgorithm *> SSBasedFactory::getSimpleESTAlgorithms() {
    return {new SSBasedDAPReachAlgorithmImplementation<Algora::SimpleESTree, true>};
}

std::vector<DynamicAPReachAlgorithm *> SSBasedFactory::getMLESTAlgorithms() {
    return {new SSBasedDAPReachAlgorithmImplementation<Algora::ESTreeML, true>};
}

std::vector<DynamicAPReachAlgorithm *> SSBasedFactory::getESTAlgorithms() {
    return {new SSBasedDAPReachAlgorithmImplementation<Algora::ESTreeQ, true>};
}

std::vector<DynamicAPReachAlgorithm *> SSBasedFactory::getOldESTAlgorithms() {
    return {new SSBasedDAPReachAlgorithmImplementation<Algora::OldESTree, true>};
}
