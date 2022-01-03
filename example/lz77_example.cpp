#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <assert.h>

#include "lz77.hpp"

// don't forget about remove memory!!!
std::size_t open_file(const char* file_name, std::uint8_t** src)
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

void save_file(const char* file_name, const std::uint8_t* src, const std::size_t src_size)
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

void print_help()
{
    std::cout << "LZ77 example:\nHelp message:\n";
    std::cout << "<PathToExecutable>/lz77_example <encode/decode> <input file name> <output file name>\n";
}

int main(int argc, char** argv)
{
    if (argc != 4) {
        print_help();
        return 0;
    }
    std::string mode        = argv[1];
    std::string input_file  = argv[2];
    std::string output_file = argv[3];

    if (mode == "encode") {
        std::uint8_t* src = nullptr;
        std::size_t file_size = open_file(input_file.c_str(), &src);

        std::uint8_t* dst = new std::uint8_t[file_size];

        std::chrono::time_point<std::chrono::high_resolution_clock> start_time = std::chrono::high_resolution_clock::now();
        std::size_t compressed_size = lz77::lz77_encode(src, file_size, dst);
        std::chrono::time_point<std::chrono::high_resolution_clock> end_time = std::chrono::high_resolution_clock::now();
        double time = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time).count();

        std::cout << "Input file size: " << file_size << " bytes;" << std::endl
                  << "Output file size: " << compressed_size << " bytes;" << std::endl
                  << "Compression ratio: " << (double)file_size / compressed_size << std::endl
                  << "Compression time: " << time << " s." << std::endl;

        save_file(output_file.c_str(), dst, compressed_size);

        delete[] dst;
        delete[] src;
    }
    else if (mode == "decode") {
        std::uint8_t* src = nullptr;
        std::size_t file_size = open_file(input_file.c_str(), &src);

        std::uint8_t* dst = new std::uint8_t[file_size * 10];

        std::chrono::time_point<std::chrono::high_resolution_clock> start_time = std::chrono::high_resolution_clock::now();
        std::size_t decompressed_size = lz77::lz77_decode(src, file_size, dst);
        std::chrono::time_point<std::chrono::high_resolution_clock> end_time = std::chrono::high_resolution_clock::now();
        double time = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time).count();

        std::cout << "Input file size: " << file_size << " bytes;" << std::endl
                  << "Output file size: " << decompressed_size << " bytes;" << std::endl
                  << "Decompression ratio: " << (double)decompressed_size / file_size << std::endl
                  << "Decompression time: " << time << " s." << std::endl;

        save_file(output_file.c_str(), dst, decompressed_size);

        delete[] dst;
        delete[] src;
    }
    else
        assert("Incorrect work mode\n");

    return 0;
}

// int main(int argc, char** argv)
// {
//     char src[] = { "abacabacabadaca" };
//     char cmp[50];
//     char dcp[20];
//     std::size_t real_size = lz77::lz77_encode((std::uint8_t*)src, sizeof(src), (std::uint8_t*)cmp);
//     std::cout << "real size - " << real_size << std::endl;
//     for (int i = 0; i < real_size; ++i)
//         std::cout << cmp[i];

//     std::size_t decompress_size = lz77::lz77_decode((std::uint8_t*)cmp, real_size, (std::uint8_t*)dcp);
//     std::cout << "decompressed size - " << decompress_size << std::endl;
//     for (int i = 0; i < decompress_size; ++i)
//         std::cout << dcp[i];
//     std::cout << std::endl;

//     return 0;
// }
