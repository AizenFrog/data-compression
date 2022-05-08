# examples
# huffman
add_executable(huffman_example huffman_example.cpp)
target_link_libraries(huffman_example PRIVATE huffman)
set_target_properties(huffman_example PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# dynamoc huffman
add_executable(dynamic_huffman_example dynamic_huffman_example.cpp)
target_link_libraries(dynamic_huffman_example PRIVATE huffman)
set_target_properties(dynamic_huffman_example PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# lz77
add_executable(lz77_example lz77_example.cpp)
target_link_libraries(lz77_example PRIVATE lz77)
set_target_properties(lz77_example PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
