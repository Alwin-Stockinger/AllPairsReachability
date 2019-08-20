cmake_minimum_required(VERSION 3.10)
project(AllPairReach)

set(CMAKE_CXX_STANDARD 17)


include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../AlgoraDyn/src/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../DyReach/src/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../AlgoraCore/src/")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/")

link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../AlgoraCore/build/Release")
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../AlgoraDyn/build/Release")
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../DyReach/build/Release")


set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -mtune=native")

SET(CONVERT src/converter/GraphFileConverter.cpp src/converter/GraphFileConverter.h )

#src
SET(DyReach "../DyReach/build/Release/randominstanceprovider.o" "../DyReach/build/Release/konectnetworkinstanceprovider.o" "../DyReach/build/Release/instanceprovider.o")
SET(ALGORITHMS src/algorithm.reach.allPair/DynamicAPReachAlgorithm.cpp src/algorithm.reach.allPair/DynamicAPReachAlgorithm.h src/algorithm.reach.allPair/DynamicSSBasedDAPReachAlgorithm.cpp src/algorithm.reach.allPair/DynamicSSBasedDAPReachAlgorithm.h src/algorithm.reach.allPair/PartitionedAPReachAlgorithm.cpp src/algorithm.reach.allPair/SimplePartitionedDAPReachAlgorithmImplementation.h src/algorithm.reach.allPair/PartitionedAPReachAlgorithm.h src/partition/Partitioner.h src/algorithm.reach.allPair/SuperVertexPDAPReachAlgorithm.h src/algorithm.reach.allPair/SuperVertexPDAPReachAlgorithm.cpp src/algorithm.reach.allPair/SimplePartitionedDAPReachAlgorithm.h src/algorithm.reach.allPair/SimplePartitionedDAPReachAlgorithm.cpp src/algorithm.reach.allPair/SuperVertexAlgorithmImplementation.h src/algorithm.reach.allPair/SSBasedDAPReachAlgorithmImplementation.h src/algorithm.reach.allPair/SSBasedDAPReachAlgorithmImplementation.cpp src/algorithm.reach.allPair/StaticSSBasedDAPReachAlgorithm.cpp src/algorithm.reach.allPair/StaticSSBasedDAPReachAlgorithm.h src/algorithm.reach.allPair/SSBasedDAPReachAlgorithm.cpp src/algorithm.reach.allPair/SSBasedDAPReachAlgorithm.h src/algorithm.reach.allPair/BFSOverlayPDAPReachAlgorithm.cpp src/algorithm.reach.allPair/BFSOverlayPDAPReachAlgorithm.h src/algorithm.reach.allPair/BFSOPDAPReachAlgorithmImplementation.h src/algorithm.reach.allPair/PartitionedDynamicAPReachAlgorithm.cpp src/algorithm.reach.allPair/PartitionedDynamicAPReachAlgorithm.h src/algorithm.reach.allPair/ReverseBFSPartitionedAPReachAlgorithm.cpp src/algorithm.reach.allPair/ReverseBFSPartitionedAPReachAlgorithm.h src/algorithm.reach.allPair/DFSOverlayPDAPReachAlgorithm.cpp src/algorithm.reach.allPair/DFSOverlayPDAPReachAlgorithm.h src/algorithm.reach.allPair/TwoWayBFSAPReachAlgorithm.cpp src/algorithm.reach.allPair/TwoWayBFSAPReachAlgorithm.h)
SET(HANDLER src/handler/factories/AlgorithmFactory.cpp src/handler/factories/AlgorithmFactory.h src/handler/FactoryFacade.h src/handler/factories/SSBasedFactory.cpp src/handler/factories/SSBasedFactory.h src/handler/factories/PartitionedFactory.cpp src/handler/factories/PartitionedFactory.h src/handler/factories/SimplePartitionFactory.cpp src/handler/factories/SimplePartitionFactory.h src/handler/factories/SuperVertexFactory.cpp src/handler/factories/SuperVertexFactory.h src/handler/AlgorithmTester.cpp src/handler/AlgorithmTester.h src/handler/factories/BFSOFactory.cpp src/handler/factories/BFSOFactory.h src/handler/factories/ReverseBFSFactory.cpp src/handler/factories/ReverseBFSFactory.h src/algorithm.reach.allPair/DSOPDAPReachAlgorithmImplementation.h src/handler/factories/DFSOFactory.cpp src/handler/factories/DFSOFactory.h src/handler/factories/TwoWayFactory.cpp src/handler/factories/TwoWayFactory.h)
SET(PARTITIONER src/partition/Partitioner.h src/partition/Partitioner.cpp)

link_libraries("libAlgoraDyn.a")
link_libraries("libAlgoraCore.a")
add_executable(AllPairReach src/main.cpp ${HANDLER} ${ALGORITHMS} ${DyReach} ${PARTITIONER})


#if(NOT(CMAKE_BUILD_TYPE STREQUAL "Release"))
    #gtest

    ADD_SUBDIRECTORY(tests/googletest)
    enable_testing()

    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


    #unit tests
    SET(TESTS tests/TestDynamicAPReachAlgorithm.cc tests/TestDynamicAPReachAlgorithm.h tests/TestSSBasedDAPReachAlgorithm.cpp tests/TestPartitionedDAPReachAlgorithm.cpp)

    add_executable(runTests ${TESTS} ${ALGORITHMS} ${PARTITIONER})
    target_link_libraries(runTests  gtest_main)
#endif()



#add_executable(converter src/converter/ConverterMain.cpp ${CONVERT})
