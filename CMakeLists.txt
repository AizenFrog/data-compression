cmake_minimum_required(VERSION 3.12)
project(data_compression_algorithms)
set (CMAKE_CXX_STANDARD 17)

# Huffman lib
add_library(huffman SHARED src/huffman_encode.cpp
                           src/huffman_decode.cpp)
target_include_directories(huffman PUBLIC include/)

#examples
add_executable(huf_example example/huffman_example.cpp)
target_link_libraries(huf_example PRIVATE huffman)