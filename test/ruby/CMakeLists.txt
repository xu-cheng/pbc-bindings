file(GLOB test_srcs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.rb")

foreach(test_src ${test_srcs})
    if(${test_src} STREQUAL "helper.rb")
        continue()
    endif()
    add_test(
        NAME ${test_src}
        COMMAND ${RUBY_EXECUTABLE} "-I${CMAKE_BINARY_DIR}/swig/ruby"
                "${CMAKE_CURRENT_SOURCE_DIR}/${test_src}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/.."
        )
endforeach()
