cmake_minimum_required(VERSION 3.16.1)

# Declaration
project(ksearch VERSION 2.1 LANGUAGES C CXX)

# some extensions of c++20 are used
set(CMAKE_CXX_STANDARD 20)

# set a numerical value to record whether the code is compiled in release mode
# or not
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_BUILD_CODE 1)
else()
    set(CMAKE_BUILD_CODE 0)
endif()

# create a variable to disable the compilation of the unit tests if desired
option(DISABLE_TESTS "Disable the compilation of the unit tests")

# Recurse into the src/ and domains/ directories
add_subdirectory (src)
add_subdirectory (domains)

# add the tests only in case they have not been disabled
if (NOT DISABLE_TESTS)
    add_subdirectory (tests)
else()
    message(STATUS "Tests disabled")
endif()

# print message warning that memory detection will not work on non-unix systems
if (NOT UNIX)
    message(NOTICE "Memory usage detection will not work on non-unix systems")
endif()
