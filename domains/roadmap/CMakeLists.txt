add_executable (roadmap
  getopt1.c
  getopt.c
  ../helpers.cc
  graph_t.cc
  roadmap_t.cc
  roadmap.cc)

target_link_libraries (roadmap LINK_PUBLIC ksearch)

add_executable (gen-roadmap
  getopt1.c
  getopt.c
  graph_t.cc
  roadmap_t.cc
  generator.cc)

target_link_libraries (gen-roadmap LINK_PUBLIC ksearch)

add_subdirectory(benchmark)