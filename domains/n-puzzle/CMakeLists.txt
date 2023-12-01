add_executable (npuzzle
  getopt1.c
  getopt.c
  ../helpers.cc
  npuzzle_t.cc
  npuzzle.cc)

target_link_libraries (npuzzle LINK_PUBLIC ksearch)

add_executable (gen-npuzzle
  getopt1.c
  getopt.c
  ../helpers.cc
  npuzzle_t.cc
  generator.cc)

target_link_libraries (npuzzle LINK_PUBLIC ksearch)
