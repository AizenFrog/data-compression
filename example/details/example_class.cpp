#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <assert.h>

#include "example_class.hpp"

example_class::example_class(const std::string& alg_name, const p_function& encode, const p_function& decode) :
    alg_name(alg_name), encode_func(encode), decode_func(decode) {}

std::size_t example_class::file_open(const std::string& file_name, std::uint8_t** src)
{
    // open file
    std::ifstream file(file_name, std::ios_base::binary);
    if (!file.is_open())
        assert("input file is not open\n");

    // file size
    file.seekg(0, std::ios_base::end);
    std::size_t file_size = file.tellg();
    file.seekg(0, std::ios_base::beg);

    // create src buffer and copy data
    *src = new std::uint8_t[file_size];
    file.read(reinterpret_cast<char*>(*src), file_size);

    // close file
    file.close();
    return file_size;
}

void example_class::save_file(const std::string& file_name, const std::uint8_t* src, const std::size_t src_size)
{
    // open or create file
    std::ofstream file(file_name, std::ios_base::binary | std::ios_base::trunc);
    if (!file.is_open())
        assert("input file is not open\n");

    // write to file
    for (std::size_t i = 0; i < src_size; ++i)
        file << src[i];

    // close file
    file.close();
}

void example_class::print_help()
{
    std::cout << alg_name << " example:\nHelp message:\n";
    std::cout << "<PathToExecutable>/huffman_example <encode/decode> <input file name> <output file name>\n";
}

void example_class::execute(const std::string& mode, const std::string& input_file, const std::string& output_file)
{
    if (mode == "encode") {
        std::uint8_t* src = nullptr;
        std::size_t file_size = file_open(input_file, &src);

        std::uint8_t* dst = new std::uint8_t[file_size];

        std::chrono::time_point<std::chrono::high_resolution_clock> start_time = std::chrono::high_resolution_clock::now();
        std::size_t compressed_size = encode_func(src, file_size, dst);
        std::chrono::time_point<std::chrono::high_resolution_clock> end_time = std::chrono::high_resolution_clock::now();
        double time = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time).count();

        std::cout << "Input file size: " << file_size << " bytes;" << std::endl
                  << "Output file size: " << compressed_size << " bytes;" << std::endl
                  << "Compression ratio: " << (double)file_size / compressed_size << std::endl
                  << "Compression time: " << time << " s." << std::endl;

        save_file(output_file, dst, compressed_size);

        delete[] dst;
        delete[] src;
    }
    else if (mode == "decode") {
        std::uint8_t* src = nullptr;
        std::size_t file_size = file_open(input_file, &src);

        std::uint8_t* dst = new std::uint8_t[file_size * 10];

        std::chrono::time_point<std::chrono::high_resolution_clock> start_time = std::chrono::high_resolution_clock::now();
        std::size_t decompressed_size = decode_func(src, file_size, dst);
        std::chrono::time_point<std::chrono::high_resolution_clock> end_time = std::chrono::high_resolution_clock::now();
        double time = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time).count();

        std::cout << "Input file size: " << file_size << " bytes;" << std::endl
                  << "Output file size: " << decompressed_size << " bytes;" << std::endl
                  << "Decompression ratio: " << (double)decompressed_size / file_size << std::endl
                  << "Decompression time: " << time << " s." << std::endl;

        save_file(output_file, dst, decompressed_size);

        delete[] dst;
        delete[] src;
    }
    else {
        print_help();
        assert("Incorrect work mode\n");
    }
}
