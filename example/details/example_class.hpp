#pragma once

#include <string>

using p_function = std::size_t (*)(const std::uint8_t*, const std::size_t, std::uint8_t*);

class example_class {
private:
    std::string alg_name;
    p_function encode_func;
    p_function decode_func;

    std::size_t file_open(const std::string& file_name, std::uint8_t** src);

    void save_file(const std::string& file_name, const std::uint8_t* src, const std::size_t src_size);

public:
    example_class(const std::string& alg_name, const p_function& encode, const p_function& decode);

    void print_help();

    void execute(const std::string& mode, const std::string& input_file, const std::string& output_file);
};
