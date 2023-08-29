add_executable (grid
  getopt1.c
  getopt.c
  ../helpers.cc
  grid_t.cc
  grid.cc)

target_link_libraries (grid LINK_PUBLIC ksearch)
