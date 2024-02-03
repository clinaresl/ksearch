cmake_minimum_required(VERSION 3.16.1)

# Declaration
project(ksearch VERSION 1.6)

# some extensions of c++20 are used
set(CMAKE_CXX_STANDARD 20)

# create a variable to disable the compilation of the unit tests if desired
option(DISABLE_TESTS "Disable the compilation of the unit tests")

# Recurse into the src/, domains/ and tests/ subdirectories
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
