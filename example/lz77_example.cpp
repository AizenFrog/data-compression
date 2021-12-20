#include <iostream>
#include <bitset>

#include "lz77.hpp"

int main(int argc, char** argv)
{
    char src[] = { "abacabacabadaca" };
    char cmp[50];
    char dcp[20];
    std::size_t real_size = lz77::lz77_encode((std::uint8_t*)src, sizeof(src), (std::uint8_t*)cmp);
    std::cout << "real size - " << real_size << std::endl;
    for (int i = 0; i < real_size; ++i)
        std::cout << std::bitset<8>(cmp[i]) << "|";

    std::size_t decompress_size = lz77::lz77_decode((std::uint8_t*)cmp, real_size, (std::uint8_t*)dcp);
    std::cout << "decompressed size - " << decompress_size << std::endl;
    for (int i = 0; i < decompress_size; ++i)
        std::cout << dcp[i];
    std::cout << std::endl;

    return 0;
}
