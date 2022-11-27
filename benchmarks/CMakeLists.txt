add_executable(benchmark main.cpp)
target_link_libraries(benchmark PUBLIC huffman)
set_target_properties(benchmark PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/benchmarks)
