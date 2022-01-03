cmake_minimum_required(VERSION 3.12)
project(data_compression_algorithms)
set (CMAKE_CXX_STANDARD 17)

# Huffman lib
add_library(huffman STATIC src/huffman_encode.cpp
                           src/huffman_decode.cpp)
target_include_directories(huffman PUBLIC include/)

# LZ77 lib
add_library(lz77 STATIC src/lz77_encode.cpp
                        src/lz77_decode.cpp)
target_include_directories(lz77 PUBLIC include/)

#examples
add_executable(huffman_example example/huffman_example.cpp)
target_link_libraries(huffman_example PRIVATE huffman)
add_executable(lz77_example example/lz77_example.cpp)
target_link_libraries(lz77_example PRIVATE lz77)
