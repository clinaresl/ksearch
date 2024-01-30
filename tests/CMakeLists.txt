enable_testing()
find_package(GTest QUIET)

if (GTest_FOUND)
    message(STATUS "Tests enabled")
else()
    message(NOTICE "GTest package not found, continuing with tests disabled")
    message(STATUS "Tests disabled")
    return()
endif()

add_executable(gtest
  gtest.cc
  TSThelpers.cc
  structs/TSTnode_t.cc
  structs/TSTbacknode_t.cc
  structs/TSTlabelednode_t.cc
  structs/TSTbucket_t.cc
  ../domains/n-pancake/npancake_t.cc
  domains/TSTnpancake.cc
  ../domains/n-puzzle/npuzzle_t.cc
  domains/TSTnpuzzle.cc
   ../domains/roadmap/graph_t.cc
  domains/TSTgraph.cc
  ../domains/roadmap/roadmap_t.cc
  domains/TSTroadmap.cc
  structs/TSTclosed_t.cc
  structs/TSTsolution_t.cc
  structs/TSTksolution_t.cc
  structs/TSTksolutions_t.cc
  algorithm/TSTmA.cc
  algorithm/TSTbela.cc)

target_link_libraries(gtest LINK_PUBLIC ksearch GTest::gtest GTest::gtest_main)

add_test(AllTests gtest)
