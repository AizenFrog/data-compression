#include <assert.h>
#include <cstring>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

#include <huffman.hpp>

struct File {
    std::uint8_t* src;
    std::size_t size;
};

inline std::size_t open_file(std::uint8_t** src, std::string& file_name) {
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

int main(int argc, char** argv) {
    // take file names
    std::vector<std::string> file_names;
    if (!std::filesystem::is_regular_file(argv[1])) {
        for (const auto& file : std::filesystem::recursive_directory_iterator(argv[1])) {
            if (std::filesystem::is_regular_file(file)) {
                file_names.push_back(std::filesystem::path(file).string());
            }
        }
    }
    else {
        assert("Given argument is not a directory");
    }

    // opening files
    std::vector<File> files(file_names.size());
    for (std::size_t i = 0; i < file_names.size(); ++i)
        files[i].size = open_file(&files[i].src, file_names[i]);

    // define benchmark constants
    const int cycles_count = 100;

    // cycle for files
    for (std::size_t i = 0; i < files.size(); ++i) {
        double comp_time = 0.0, decomp_time = 0.0;
        std::uint8_t* dst_compression   = new std::uint8_t[files[i].size];
        std::uint8_t* dst_decompression = new std::uint8_t[files[i].size];
        for (int j = 0; j < cycles_count; ++j) {
            std::memset(dst_compression, 0, files[i].size);
            std::chrono::time_point<std::chrono::high_resolution_clock> start_time = std::chrono::high_resolution_clock::now();
            std::size_t compressed_size = huffman::huffman_encode(files[i].src, files[i].size, dst_compression);
            std::chrono::time_point<std::chrono::high_resolution_clock> end_time = std::chrono::high_resolution_clock::now();
            comp_time += std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time).count();

            std::memset(dst_decompression, 0, files[i].size);
            start_time = std::chrono::high_resolution_clock::now();
            std::size_t decompressed_size = huffman::huffman_decode(dst_compression, compressed_size, dst_decompression);
            end_time = std::chrono::high_resolution_clock::now();
            decomp_time += std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time).count();
        }
        comp_time /= cycles_count;
        decomp_time /= cycles_count;
        std::cout << file_names[i] << ":\tcompression time - " << comp_time << "s|\tdecompression time - " << decomp_time << "s" << std::endl;
        delete[] dst_compression;
    }

    // clear memory
    for (File& file : files)
        delete[] file.src;

    return 0;
}
