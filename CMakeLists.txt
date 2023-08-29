cmake_minimum_required(VERSION 3.27.1)

# Declaration
project(ksearch VERSION 1.3)

# some extensions of c++17 are used
set(CMAKE_CXX_STANDARD 17)

# Recurse into the src/, domains/ and tests/ subdirectories
add_subdirectory (src)
add_subdirectory (domains)
add_subdirectory (tests)
