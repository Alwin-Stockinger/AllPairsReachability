//
// Created by Alwin Stockinger.
//

#include "../src/algorithm.reach.allPair/StaticBFSAPReachAlgorithm.h"

#include "gtest/gtest.h"

#include "TestDynamicAPReachAlgorithm.h"

INSTANTIATE_TEST_SUITE_P(StaticBFSAPREACHAlgorithm,TestDynamicAPReachAlgorithm, ::testing::Values(new StaticBFSAPReachAlgorithm));
