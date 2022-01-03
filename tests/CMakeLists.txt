# adding gtest library
add_subdirectory(${CMAKE_SOURCE_DIR}/3rd_party/gtest ${CMAKE_BINARY_DIR}/3rd_party/gtest EXCLUDE_FROM_ALL)

# adding tests
add_executable(tests main.cpp
                     huffman_tests.cpp)
target_link_libraries(tests PUBLIC huffman gtest)
set_target_properties(tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests)
