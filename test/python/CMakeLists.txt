find_package(PythonInterp 3.3 REQUIRED)
file(GLOB test_srcs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.py")

foreach(test_src ${test_srcs})
    if(${test_src} EQUAL "helper.py")
        continue()
    endif()
    add_test(
        NAME ${test_src}
        COMMAND /usr/bin/env "PYTHONPATH=${CMAKE_BINARY_DIR}"
            ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/${test_src}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/.."
        )
endforeach()
