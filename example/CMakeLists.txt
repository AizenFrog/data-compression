# examples
function(add_example algorithm)
    set(example_name ${algorithm}_example)
    add_executable(${example_name} ${example_name}.cpp "details/example_class.cpp")
    target_link_libraries(${example_name} PRIVATE ${ARGN})
    set_target_properties(${example_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endfunction(add_example)

# huffman
add_example(huffman huffman)

# dynamic huffman
add_example(dynamic_huffman huffman)

# lz77
add_example(lz77 lz77)
