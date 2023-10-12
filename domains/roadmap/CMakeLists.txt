add_executable (roadmap
  getopt1.c
  getopt.c
  ../helpers.cc
  graph_t.cc
  roadmap.cc)

target_link_libraries (roadmap LINK_PUBLIC ksearch)
