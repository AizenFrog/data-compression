#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <bitset>
#include <assert.h>

#include "huffman.hpp"

int main(int argc, char** argv)
{
    char src[] = { "beep boop beer!" };
    char cmp[200];
    char dcp[50];
    std::memset(cmp, 0, 200);
    std::size_t real_size = huffman::huffman_encode((std::uint8_t*)src, sizeof(src), (std::uint8_t*)cmp);
    std::cout << "compressed size - " << real_size << std::endl;
    for (int i = 0; i < real_size; ++i)
        std::cout << std::bitset<8>(cmp[i]) << "|";
    std::cout << std::endl;

    std::size_t decompressed_size = huffman::huffman_decode((std::uint8_t*)cmp, real_size, (std::uint8_t*)dcp);
    std::cout << "decompressed size - " << decompressed_size << std::endl;
    for (int i = 0; i < decompressed_size; ++i)
        std::cout << dcp[i];
    std::cout << std::endl;

    // std::ifstream file(argv[1], std::ios::binary);
    // if (!file.is_open()) {
    //     assert("file is not open");
    // }
    // file.seekg(0, std::ios_base::beg);
    // while (!file.eof()) {

    // }


    return 0;
}