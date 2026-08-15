# Create a library called libksearch which includes its source files
add_library (ksearch
  ksearch.h
  KHSdefs.h
  structs/KHSnode_t.h
  structs/KHSbacknode_t.h
  structs/KHSlabelednode_t.h
  structs/KHSidnode_t.h
  structs/KHSbucket_t.h
  structs/KHSsolution_t.h
  structs/KHSksolution_t.h
  structs/KHSIBaseNode.h
  structs/KHSiomanip.h
  algorithm/KHSbsolver.h
  algorithm/KHSmA.h
  algorithm/KHSbela.h
  algorithm/KHSbbela.h
  algorithm/KHSsbela.h
  algorithm/KHSIK.h)

# Generate the main header file from its template
configure_file(ksearch.h.in ${CMAKE_CURRENT_SOURCE_DIR}/ksearch.h)

# also determine the current version tag from github
find_package(Git QUIET)

# in case git is available in the source installation then compute its value
# from "git describe"
if(Git_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
    execute_process(
        COMMAND
            "${GIT_EXECUTABLE}" describe --tags --always --dirty
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/src"
        OUTPUT_VARIABLE KSEARCH_GIT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
      )

# otherwise, retrieve it from a file called VERSION which should then be created
# in a source installation that actually uses git. Of course, this file could
# contain free strings but it is recommended to create it with the output of:
#
#    git describe --tags --always --dirty > git_version
elseif(EXISTS "${CMAKE_SOURCE_DIR}/src/git_version")
    file(STRINGS "${CMAKE_SOURCE_DIR}/src/git_version" KSEARCH_GIT_VERSION
         LIMIT_COUNT 1)

# if neither git is available in the source installation nor a file called
# "version" is found, then just simply report "unknown git version"
else()
    set(KSEARCH_GIT_VERSION "unknown git version")
endif()

configure_file(version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/version.h)

set_target_properties(ksearch PROPERTIES LINKER_LANGUAGE CXX)

# Make sure the compiler can find include files for the library when other
# libraries or executables link to it
target_include_directories (ksearch PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
