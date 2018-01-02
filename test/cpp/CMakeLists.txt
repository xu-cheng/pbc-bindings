file(GLOB test_srcs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")

foreach(test_src ${test_srcs})
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} ${test_src})
    target_link_libraries(${test_name} ${linked_libraries})
    set_target_properties(${test_name}
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_TEST_OUTPUT_DIRECTORY})
    target_include_directories(${test_name}
        PUBLIC "${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/deps/include")
    add_test(
        NAME ${test_name}
        COMMAND "${CMAKE_TEST_OUTPUT_DIRECTORY}/${test_name}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/.."
        )
endforeach()
