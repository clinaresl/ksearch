add_executable (map
  getopt1.c
  getopt.c
  ../helpers.cc
  map_t.cc
  map.cc)

target_link_libraries (map LINK_PUBLIC ksearch)
