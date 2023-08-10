enable_testing()
find_package(GTest REQUIRED)

add_executable(gtest
  gtest.cc
  TSThelpers.cc
  structs/TSTnode_t.cc
  structs/TSTbacknode_t.cc
  structs/TSTbucket_t.cc
  ../domains/n-pancake/npancake_t.cc
  domains/TSTnpancake.cc
  structs/TSTclosed_t.cc
  structs/TSTsolution_t.cc
  structs/TSTksolution_t.cc
  structs/TSTksolutions_t.cc
  algorithm/TSTmA.cc)

target_link_libraries(gtest LINK_PUBLIC ksearch GTest::gtest GTest::gtest_main)

add_test(AllTests gtest)
