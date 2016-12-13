cmake_minimum_required (VERSION 3.7.0)
project("pbc-bindings")

# set output directories.
set(CMAKE_TEST_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test)

# this is the directory for our custom CMake modules.
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# prefer to Homebrew formulae over system Framework on macOS.
if(APPLE)
    set(CMAKE_FIND_FRAMEWORK LAST)
endif()

find_package(PBC REQUIRED)
include_directories(${PBC_INCLUDE_DIRS})
list(APPEND linked_libraries ${PBC_LIBRARIES})

find_package(GMP REQUIRED)
include_directories(${GMP_INCLUDE_DIRS})
list(APPEND linked_libraries ${GMP_LIBRARIES} ${GMPXX_LIBRARIES})

# enable c++14.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

# option to enable SWIG bindings
option(ENABLE_PYTHON "Enable SWIG Python 3 bindings" OFF)
option(ENABLE_RUBY "Enable SWIG Ruby bindings" OFF)

# option to enable test
option(ENABLE_TEST "Enable test" OFF)

if(ENABLE_PYTHON OR ENABLE_RUBY)
    find_package(SWIG REQUIRED)
    include(${SWIG_USE_FILE})

    include_directories("${PROJECT_SOURCE_DIR}/include")
    set_source_files_properties("swig/pbc.i" PROPERTIES CPLUSPLUS ON)
endif()

if(ENABLE_PYTHON)
    find_package(PythonLibs 3.3 REQUIRED)
    include_directories(${PYTHON_INCLUDE_PATH})

    set(CMAKE_SWIG_OUTDIR "${CMAKE_BINARY_DIR}/swig/python")
    swig_add_module(pbc python swig/pbc.i)
    swig_link_libraries(pbc ${linked_libraries} ${PYTHON_LIBRARIES})
    set_target_properties(${SWIG_MODULE_pbc_REAL_NAME}
        PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SWIG_OUTDIR})
endif()

if(ENABLE_RUBY)
    find_package(Ruby 2.0 REQUIRED)
    include_directories(${RUBY_INCLUDE_DIRS})

    set(CMAKE_SWIG_OUTDIR "${CMAKE_BINARY_DIR}/swig/ruby")
    swig_add_module(pbc ruby swig/pbc.i)
    swig_link_libraries(pbc ${linked_libraries} ${RUBY_LIBRARY})
    set_target_properties(${SWIG_MODULE_pbc_REAL_NAME}
        PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SWIG_OUTDIR})
endif()

if(ENABLE_TEST)
    enable_testing()
    add_subdirectory(test/cpp)

    if(ENABLE_PYTHON)
        add_subdirectory(test/python)
    endif()
    if(ENABLE_RUBY)
        add_subdirectory(test/ruby)
    endif()
endif()
