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

#include "gtest/gtest.h"

#include "TestDynamicAPReachAlgorithm.h"

#include "../src/algorithm.reach.allPair/SSBasedDAPReachAlgorithmImplementation.h"

#include <algorithm.reach/staticbfsssreachalgorithm.h>
#include <algorithm.reach/staticdfsssreachalgorithm.h>
#include <algorithm.reach/cachingbfsssreachalgorithm.h>
#include <algorithm.reach/cachingdfsssreachalgorithm.h>
#include <algorithm.reach/lazydfsssreachalgorithm.h>
#include <algorithm.reach/lazybfsssreachalgorithm.h>
#include <algorithm.reach/simpleincssreachalgorithm.h>
#include <algorithm.reach.es/estree-ml.h>
#include <algorithm.reach.es/estree-bqueue.h>
#include <algorithm.reach.es/estree-queue.h>
#include <algorithm.reach.es/simpleestree.h>

#include "../src/algorithm.reach.allPair/TwoWayBFSAPReachAlgorithm.h"

INSTANTIATE_TEST_SUITE_P(SSBasedDAPReachAlgorithmImplementationTests,TestDynamicAPReachAlgorithm, ::testing::Values(new SSBasedDAPReachAlgorithmImplementation<Algora::StaticDFSSSReachAlgorithm>,new SSBasedDAPReachAlgorithmImplementation<Algora::StaticBFSSSReachAlgorithm>, new SSBasedDAPReachAlgorithmImplementation<Algora::CachingBFSSSReachAlgorithm>, new SSBasedDAPReachAlgorithmImplementation<Algora::CachingDFSSSReachAlgorithm>, new SSBasedDAPReachAlgorithmImplementation<Algora::LazyDFSSSReachAlgorithm>, new SSBasedDAPReachAlgorithmImplementation<Algora::LazyBFSSSReachAlgorithm>, new SSBasedDAPReachAlgorithmImplementation<Algora::SimpleIncSSReachAlgorithm>, new SSBasedDAPReachAlgorithmImplementation<Algora::ESTreeQ>, new SSBasedDAPReachAlgorithmImplementation<Algora::OldESTree>, new SSBasedDAPReachAlgorithmImplementation<Algora::ESTreeML>, new SSBasedDAPReachAlgorithmImplementation<Algora::SimpleESTree>, new TwoWayBFSAPReachAlgorithm));