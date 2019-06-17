//
// Created by Alwin Stockinger.
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

INSTANTIATE_TEST_SUITE_P(SSBasedDAPReachAlgorithmImplementationTests,TestDynamicAPReachAlgorithm, ::testing::Values(new SSBasedDAPReachAlgorithmImplementation<Algora::StaticDFSSSReachAlgorithm>,new SSBasedDAPReachAlgorithmImplementation<Algora::StaticBFSSSReachAlgorithm>, new SSBasedDAPReachAlgorithmImplementation<Algora::CachingBFSSSReachAlgorithm>, new SSBasedDAPReachAlgorithmImplementation<Algora::CachingDFSSSReachAlgorithm>, new SSBasedDAPReachAlgorithmImplementation<Algora::LazyDFSSSReachAlgorithm>, new SSBasedDAPReachAlgorithmImplementation<Algora::LazyBFSSSReachAlgorithm>, new SSBasedDAPReachAlgorithmImplementation<Algora::SimpleIncSSReachAlgorithm>, new SSBasedDAPReachAlgorithmImplementation<Algora::ESTreeQ>, new SSBasedDAPReachAlgorithmImplementation<Algora::OldESTree>, new SSBasedDAPReachAlgorithmImplementation<Algora::ESTreeML>, new SSBasedDAPReachAlgorithmImplementation<Algora::SimpleESTree>));