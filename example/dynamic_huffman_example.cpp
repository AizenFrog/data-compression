#include <iostream>
#include <cstring>
#include <bitset>
#include <huffman.hpp>

int main(int argc, char** argv)
{
    std::size_t buffer_size = 500;
    char src[] = { "hello world!" };
    char buffer[buffer_size];
    char dst[sizeof(src)];
    std::memset(buffer, 0, buffer_size);
    std::cout << "real size - " << sizeof(src) << std::endl;
    std::size_t real_size = huffman::dynamic_huffman_encode((std::uint8_t*)src, sizeof(src), (std::uint8_t*)buffer);
    std::cout << "compress is completed!\n";
    for (std::size_t i = 0; i < real_size; ++i)
        std::cout << std::bitset<8>(buffer[i]) << "|";
    std::cout << std::endl;
    std::size_t decompressed_size = huffman::dynamic_huffman_decode((std::uint8_t*)buffer, real_size, (std::uint8_t*)dst);
    std::cout << "decompress size - " << decompressed_size << std::endl;
    for (std::size_t i = 0; i < sizeof(src); ++i)
        std::cout << char(dst[i]);
    std::cout << std::endl;
    return 0;
}
