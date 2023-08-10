# Create a library called libksearch which includes its source files
add_library (ksearch
  ksearch.h
  KHSdefs.h
  structs/KHSnode_t.h
  structs/KHSbacknode_t.h
  structs/KHSbucket_t.h
  structs/KHSsolution_t.h
  structs/KHSksolution_t.h
  algorithm/KHSmA.h)

# Generate the main header file from its template
configure_file(ksearch.h.in ./ksearch.h)

set_target_properties(ksearch PROPERTIES LINKER_LANGUAGE CXX)

# Make sure the compiler can find include files for the library when other
# libraries or executables link to it
target_include_directories (ksearch PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
