enable_testing()
find_package(GTest REQUIRED)

add_executable(gtest
  gtest.cc
  TSThelpers.cc
  # ../domains/n-puzzle/npuzzle_t.cc
  structs/TSTnode_t.cc
  structs/TSTbacknode_t.cc
  structs/TSTbucket_t.cc)
  # structs/TSTclosed_t.cc
  # structs/TSTclassical_perimeter_t.cc
  # structs/TSTcompressed_perimeter_t.cc
  # structs/TSTsolution_t.cc
  # structs/TSTsolutions_t.cc
  # domains/TSTnpuzzle.cc
  # algorithm/TSTida.cc
  # algorithm/TSTbida.cc)

target_link_libraries(gtest LINK_PUBLIC ksearch GTest::gtest GTest::gtest_main)

add_test(AllTests gtest)
