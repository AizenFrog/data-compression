# adding gtest library
if (WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()
add_subdirectory(${CMAKE_SOURCE_DIR}/3rd_party/gtest ${CMAKE_BINARY_DIR}/3rd_party/gtest EXCLUDE_FROM_ALL)

# adding tests
add_executable(tests main.cpp
                     huffman_tests.cpp
                     dynamic_huffman_tests.cpp
                     lz77_tests.cpp)
target_link_libraries(tests PUBLIC huffman lz77 gtest)
set_target_properties(tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests)
