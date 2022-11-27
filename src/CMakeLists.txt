find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# Huffman lib
add_library(huffman STATIC huffman_encode.cpp
                           huffman_decode.cpp
                           dynamic_huffman_encode.cpp
                           dynamic_huffman_decode.cpp)
target_include_directories(huffman PUBLIC ${CMAKE_SOURCE_DIR}/include/)
#target_link_libraries(huffman PUBLIC OpenMP::OpenMP_CXX)
set_target_properties(huffman PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
target_compile_options(huffman PRIVATE -mavx2 -mavx512vl -mavx512f)

# LZ77 lib
add_library(lz77 STATIC lz77_encode.cpp
                        lz77_decode.cpp)
target_include_directories(lz77 PUBLIC ${CMAKE_SOURCE_DIR}/include/)
set_target_properties(lz77 PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
