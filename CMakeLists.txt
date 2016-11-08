cmake_minimum_required (VERSION 3.6.0)
project("pbc-bindings")

# set output directories.
set(CMAKE_TEST_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test)

# this is the directory for our custom CMake modules.
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(PBC REQUIRED)
include_directories(${PBC_INCLUDE_DIRS})
list(APPEND linked_libraries ${PBC_LIBRARIES})

find_package(GMP REQUIRED)
include_directories(${GMP_INCLUDE_DIRS})
list(APPEND linked_libraries ${GMP_LIBRARIES})

# enable c++14.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

# option to enable SWIG bindings
option(ENABLE_PYTHON "Enable SWIG Python 3 bindings" ON)

# option to enable test
option(ENABLE_TEST "Enable test" OFF)

if(ENABLE_PYTHON)
    find_package(SWIG REQUIRED)
    include(${SWIG_USE_FILE})

    if(APPLE)
        # Patching CMakeFindFrameworks.cmake
        # TODO: remove it after cmake 3.7.0 is released.
        set(CMAKE_FIND_FRAMEWORKS_INCLUDED 1)
        macro(CMAKE_FIND_FRAMEWORKS fwk)
            set(${fwk}_FRAMEWORKS)
            foreach(dir
                ~/Library/Frameworks/${fwk}.framework
                /Library/Frameworks/${fwk}.framework
                /usr/local/Frameworks/${fwk}.framework
                /System/Library/Frameworks/${fwk}.framework
                /Network/Library/Frameworks/${fwk}.framework)
                if(EXISTS ${dir})
                    set(${fwk}_FRAMEWORKS ${${fwk}_FRAMEWORKS} ${dir})
                endif()
            endforeach()
        endmacro()
    endif()

    find_package(PythonLibs 3.3 REQUIRED)
    include_directories(${PYTHON_INCLUDE_PATH})

    include_directories("${PROJECT_SOURCE_DIR}/include")
    set_source_files_properties("swig/pbc.i" PROPERTIES CPLUSPLUS ON)
    swig_add_module(pbc python swig/pbc.i)
    swig_link_libraries(pbc ${linked_libraries} ${PYTHON_LIBRARIES})
endif()

if(ENABLE_TEST)
    enable_testing()
    add_subdirectory(test)
endif()
