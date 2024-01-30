add_executable (map
  getopt1.c
  getopt.c
  ../helpers.cc
  map_t.cc
  map.cc)

target_link_libraries (map LINK_PUBLIC ksearch)

add_executable (gen-map
  getopt1.c
  getopt.c
  map_t.cc
  generator.cc //cse-server1/UDfiles/iherman/Documents/CLionProjects/libksearch/domains/roadmap/roadmap.cc)

target_link_libraries (gen-map LINK_PUBLIC ksearch)
