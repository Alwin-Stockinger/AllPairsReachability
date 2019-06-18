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




SET(CONVERT src/converter/GraphFileConverter.cpp src/converter/GraphFileConverter.h )

#src
SET(OBJS "../DyReach/build/Release/randominstanceprovider.o" "../DyReach/build/Release/konectnetworkinstanceprovider.o" "../DyReach/build/Release/reachabilitysuite.o" src/algorithm.reach.allPair/DynamicAPReachAlgorithm.cpp src/algorithm.reach.allPair/DynamicAPReachAlgorithm.h src/algorithm.reach.allPair/SSBasedDAPReachAlgorithm.cpp src/algorithm.reach.allPair/SSBasedDAPReachAlgorithm.h src/algorithm.reach.allPair/PartitionedDAPReachAlgorithm.cpp src/algorithm.reach.allPair/PartitionedDAPReachAlgorithmImplementation.h src/handler/AlgorithmHandler.cpp src/handler/AlgorithmHandler.h src/algorithm.reach.allPair/PartitionedDAPReachAlgorithm.h src/generator/RandomDoubleQueryGenerator.cpp src/generator/RandomDoubleQueryGenerator.h src/algorithm.reach.allPair/SSBasedDAPReachAlgorithmImplementation.h)

link_libraries("libAlgoraDyn.a")
link_libraries("libAlgoraCore.a")
add_executable(AllPairReach src/main.cpp ${OBJS} ${CONVERT})





#gtest

ADD_SUBDIRECTORY(tests/googletest)
enable_testing()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


#unit tests
SET(TESTS tests/TestDynamicAPReachAlgorithm.cc tests/TestDynamicAPReachAlgorithm.h tests/TestSSBasedDAPReachAlgorithm.cpp tests/TestPartitionedDAPReachAlgorithm.cpp src/generator/RandomDoubleQueryGenerator.cpp src/generator/RandomDoubleQueryGenerator.h src/algorithm.reach.allPair/PartitionedDAPReachAlgorithm.h)

add_executable(runTests ${OBJS} ${TESTS} ${CONVERT})

target_link_libraries(runTests  gtest_main)


add_executable(converter src/converter/ConverterMain.cpp ${CONVERT})
