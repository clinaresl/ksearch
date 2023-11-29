add_executable (npancake
  getopt1.c
  getopt.c
  ../helpers.cc
  npancake_t.cc
  npancake.cc)

target_link_libraries (npancake LINK_PUBLIC ksearch)

add_executable (gen-npancake
  getopt1.c
  getopt.c
  ../helpers.cc
  npancake_t.cc
  generator.cc)

target_link_libraries (npancake LINK_PUBLIC ksearch)
