cmake_minimum_required(VERSION 3.12)
project(data_compression_algorithms)
set (CMAKE_CXX_STANDARD 17)

# compression libraries
add_subdirectory(src)

# adding tests
add_subdirectory(tests)

# aading benchmarks
add_subdirectory(benchmarks)

#examples
add_subdirectory(example)
