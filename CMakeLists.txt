cmake_minimum_required(VERSION 3.16.1)

# Declaration
project(ksearch VERSION 1.6)

# some extensions of c++17 are used
set(CMAKE_CXX_STANDARD 17)

# create a variable to disable the compilation of the unit tests if desired
option(DISABLE_TESTS "Disable the compilation of the unit tests")

# Recurse into the src/, domains/ and tests/ subdirectories
add_subdirectory (src)
add_subdirectory (domains)

# add the tests only in case they have not been disabled
if (NOT DISABLE_TESTS)
    message(STATUS "Tests enabled")
    add_subdirectory (tests)
else()
    message(STATUS "Tests disabled")
endif()
