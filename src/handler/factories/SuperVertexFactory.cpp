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

#include "SuperVertexFactory.h"

#include <algorithm.reach/lazydfsssreachalgorithm.h>
#include <algorithm.reach.es/simpleestree.h>
#include <algorithm.reach.es/estree-queue.h>
#include <algorithm.reach.es/estree-ml.h>
#include <algorithm.reach.es/estree-bqueue.h>
#include <algorithm.reach/lazybfsssreachalgorithm.h>
#include <algorithm.reach/cachingdfsssreachalgorithm.h>
#include <algorithm.reach/cachingbfsssreachalgorithm.h>
#include <algorithm.reach/simpleincssreachalgorithm.h>
#include "../../algorithm.reach.allPair/SSBasedDAPReachAlgorithm.h"
#include "../../algorithm.reach.allPair/SSBasedDAPReachAlgorithmImplementation.h"
#include "../../algorithm.reach.allPair/SuperVertexAlgorithmImplementation.h"

std::vector<DynamicAPReachAlgorithm *> SuperVertexFactory::getStaticBFSAlgorithms() {
    return createAlgorithms<Algora::StaticBFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> SuperVertexFactory::getStaticDFSAlgorithms() {
    return createAlgorithms<Algora::StaticDFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> SuperVertexFactory::getCachingBFSAlgorithms() {
    return createAlgorithms<Algora::CachingBFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> SuperVertexFactory::getCachingDFSAlgorithms() {
    return createAlgorithms<Algora::CachingDFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> SuperVertexFactory::getLazyBFSAlgorithms() {
    return createAlgorithms<Algora::LazyBFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> SuperVertexFactory::getLazyDFSAlgorithms() {
    return createAlgorithms<Algora::LazyDFSSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> SuperVertexFactory::getSimpleISSRAlgorithms() {
    return createAlgorithms<Algora::SimpleIncSSReachAlgorithm>();
}

std::vector<DynamicAPReachAlgorithm *> SuperVertexFactory::getSimpleESTAlgorithms() {
    return createAlgorithms<Algora::SimpleESTree>();
}

std::vector<DynamicAPReachAlgorithm *> SuperVertexFactory::getMLESTAlgorithms() {
    return createAlgorithms<Algora::ESTreeML>();
}

std::vector<DynamicAPReachAlgorithm *> SuperVertexFactory::getESTAlgorithms() {
    return createAlgorithms<Algora::ESTreeQ>();
}

std::vector<DynamicAPReachAlgorithm *> SuperVertexFactory::getOldESTAlgorithms() {
    return createAlgorithms<Algora::OldESTree>();
}

template<typename SubAlgorithm>
std::vector<DynamicAPReachAlgorithm *> SuperVertexFactory::createAlgorithms() {

    std::vector<DynamicAPReachAlgorithm*> algorithms;

    for(const std::string& overlayName : overlayNames) {

        std::vector<DynamicAPReachAlgorithm*> specificAlgorithms;

        if (overlayName == "StaticBFS") {
            specificAlgorithms =
                    createSpecificAlgorithms<SubAlgorithm, Algora::StaticBFSSSReachAlgorithm>();
        } else if (overlayName == "StaticDFS") {
            specificAlgorithms =
                    createSpecificAlgorithms <SubAlgorithm, Algora::StaticDFSSSReachAlgorithm >();
        } else if (overlayName == "LazyDFS") {
            specificAlgorithms =
                    createSpecificAlgorithms <SubAlgorithm, Algora::LazyDFSSSReachAlgorithm> ();
        } else if (overlayName == "LazyBFS") {
            specificAlgorithms =
                    createSpecificAlgorithms <SubAlgorithm, Algora::LazyBFSSSReachAlgorithm> ();
        } else if (overlayName == "CachingDFS") {
            specificAlgorithms =
                    createSpecificAlgorithms <SubAlgorithm,Algora::CachingDFSSSReachAlgorithm> ();
        } else if (overlayName == "CachingBFS") {
            specificAlgorithms =
                    createSpecificAlgorithms <SubAlgorithm, Algora::CachingBFSSSReachAlgorithm> ();
        } else if (overlayName == "SimpleInc") {
            specificAlgorithms =
                    createSpecificAlgorithms <SubAlgorithm, Algora::SimpleIncSSReachAlgorithm> ();
        } else if (overlayName == "ESTreeML") {
            specificAlgorithms =
                    createSpecificAlgorithms <SubAlgorithm, Algora::ESTreeML> ();
        } else if (overlayName == "OldESTree") {
            specificAlgorithms =
                    createSpecificAlgorithms <SubAlgorithm, Algora::OldESTree> ();
        } else if (overlayName == "ESTreeQ") {
            specificAlgorithms =
                    createSpecificAlgorithms <SubAlgorithm, Algora::ESTreeQ> ();
        } else if (overlayName == "SimpleESTree") {
            specificAlgorithms =
                    createSpecificAlgorithms <SubAlgorithm, Algora::SimpleESTree> ();
        } else {
            throw std::invalid_argument(overlayName + " is not a viable algorithm");
        }

        algorithms.insert(algorithms.end(), specificAlgorithms.begin(), specificAlgorithms.end());
    }
    return algorithms;
}

template<typename SubAlgorithm, typename OverlayAlgorithm>
std::vector<DynamicAPReachAlgorithm *> SuperVertexFactory::createSpecificAlgorithms() {

    std::vector<DynamicAPReachAlgorithm*> algorithms;

    for(unsigned long long k = kMin; k <= kMax ;  [this, &k]() {
        exponentialK ? (k *= 2ULL) : (k++);
    }()) {
        for(unsigned long long depth = depthMin; depth <= depthMax; depth++){
            auto* algorithm = new SuperVertexAlgorithmImplementation<SSBasedDAPReachAlgorithmImplementation<SubAlgorithm, true>, OverlayAlgorithm>(depth);

            configureAlgorithm(algorithm);
            algorithm->setK(k);

            algorithms.push_back(algorithm);
        }
    }
    return algorithms;
}