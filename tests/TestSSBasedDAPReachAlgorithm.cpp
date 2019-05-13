//
// Created by Alwin Stockinger.
//

#include "gtest/gtest.h"

#include "TestDynamicAPReachAlgorithm.h"

#include "../src/algorithm.reach.allPair/SSBasedDAPReachAlgorithm.h"

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

INSTANTIATE_TEST_SUITE_P(SSBasedDAPReachAlgorithmTests,TestDynamicAPReachAlgorithm, ::testing::Values(new SSBasedDAPReachAlgorithm<Algora::StaticDFSSSReachAlgorithm>,new SSBasedDAPReachAlgorithm<Algora::StaticBFSSSReachAlgorithm>, new SSBasedDAPReachAlgorithm<Algora::CachingBFSSSReachAlgorithm>, new SSBasedDAPReachAlgorithm<Algora::CachingDFSSSReachAlgorithm>, new SSBasedDAPReachAlgorithm<Algora::LazyDFSSSReachAlgorithm>, new SSBasedDAPReachAlgorithm<Algora::LazyBFSSSReachAlgorithm>, new SSBasedDAPReachAlgorithm<Algora::SimpleIncSSReachAlgorithm>, new SSBasedDAPReachAlgorithm<Algora::ESTreeQ>, new SSBasedDAPReachAlgorithm<Algora::OldESTree>, new SSBasedDAPReachAlgorithm<Algora::ESTreeML>, new SSBasedDAPReachAlgorithm<Algora::SimpleESTree>));