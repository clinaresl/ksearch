enable_testing()
find_package(GTest REQUIRED)

add_executable(gtest
  gtest.cc
  TSThelpers.cc
  structs/TSTnode_t.cc
  structs/TSTbacknode_t.cc
  structs/TSTbucket_t.cc
  structs/TSTclosed_t.cc)

target_link_libraries(gtest LINK_PUBLIC ksearch GTest::gtest GTest::gtest_main)

add_test(AllTests gtest)
