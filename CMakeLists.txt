cmake_minimum_required(VERSION 3.13)
project(AllPairReach)

set(CMAKE_CXX_STANDARD 17)


include_directories("../AlgoraDyn/src/")
include_directories("../DyReach/src/")
include_directories("../AlgoraCore/src/")

link_directories("../AlgoraDyn/build/Release")
link_directories("../AlgoraCore/build/Release")
link_directories("../DyReach/build/Release")

link_libraries("libAlgoraDyn.a")
link_libraries("libAlgoraCore.a")

SET(OBJS "../DyReach/build/Release/randominstanceprovider.o" "../DyReach/build/Release/reachabilitysuite.o" src/algorithm.reach.allPair/DynamicAPReachAlgorithm.cpp src/algorithm.reach.allPair/DynamicAPReachAlgorithm.h src/algorithm.reach.allPair/SSBasedDAPReachAlgorithm.cpp src/algorithm.reach.allPair/SSBasedDAPReachAlgorithm.h src/algorithm.reach.allPair/StaticBFSAPReachAlgorithm.cpp src/algorithm.reach.allPair/StaticBFSAPReachAlgorithm.h src/algorithm.reach.allPair/StaticDFSAPReachAlgorithm.cpp src/algorithm.reach.allPair/StaticDFSAPReachAlgorithm.h src/algorithm.reach.allPair/CachingBFSAPReachAlgorithm.cpp src/algorithm.reach.allPair/CachingBFSAPReachAlgorithm.h src/algorithm.reach.allPair/CachingDFSAPReachAlgorithm.cpp src/algorithm.reach.allPair/CachingDFSAPReachAlgorithm.h src/algorithm.reach.allPair/LazyBFSAPReachAlgorithm.cpp src/algorithm.reach.allPair/LazyBFSAPReachAlgorithm.h src/algorithm.reach.allPair/LazyDFSAPReachAlgorithm.cpp src/algorithm.reach.allPair/LazyDFSAPReachAlgorithm.h src/algorithm.reach.allPair/SimpleIncAPReachAlgorithm.cpp src/algorithm.reach.allPair/SimpleIncAPReachAlgorithm.h)

add_executable(AllPairReach src/main.cpp ${OBJS})


#gtest

ADD_SUBDIRECTORY(tests/googletest)
enable_testing()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


#unit tests
SET(TESTS tests/TestDynamicAPReachAlgorithm.cc tests/TestDynamicAPReachAlgorithm.h tests/TestStaticBFSAPReachAlgorithm.cpp)

add_executable(runTests ${TESTS} ${OBJS})

target_link_libraries(runTests  gtest_main)
