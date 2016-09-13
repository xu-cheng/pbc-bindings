cmake_minimum_required (VERSION 3.6.0)
project("pbc-bindings")

# set output directories.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_TEST_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test)

# this is the directory for our custom CMake modules.
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(PBC REQUIRED)
include_directories(${PBC_INCLUDE_DIRS})
list(APPEND linked_libraries ${PBC_LIBRARIES})

# enable c++14.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

# add unit test
enable_testing()
add_subdirectory(test)
