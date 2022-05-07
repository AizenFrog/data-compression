# Huffman lib
add_library(huffman STATIC huffman_encode.cpp
                           huffman_decode.cpp
                           dynamic_huffman_encode.cpp
                           dynamic_huffman_decode.cpp)
target_include_directories(huffman PUBLIC ${CMAKE_SOURCE_DIR}/include/)
set_target_properties(huffman PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# LZ77 lib
add_library(lz77 STATIC lz77_encode.cpp
                        lz77_decode.cpp)
target_include_directories(lz77 PUBLIC ${CMAKE_SOURCE_DIR}/include/)
set_target_properties(lz77 PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
