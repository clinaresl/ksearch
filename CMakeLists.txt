cmake_minimum_required(VERSION 3.26.1)

# Declaration
project(ksearch VERSION 1.4)

# some extensions of c++17 are used
set(CMAKE_CXX_STANDARD 20)

# Recurse into the src/, domains/ and tests/ subdirectories
add_subdirectory (src)
add_subdirectory (domains)
add_subdirectory (tests)
