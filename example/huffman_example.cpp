#include <cstring>

#include "huffman.hpp"

int main(int argc, char** argv)
{
    char src[] = { "beep boop beer!" };
    char dst[20];
    std::memset(dst, 0, 20);
    std::size_t real_size = huffman::huffman_encode((std::uint8_t*)src, sizeof(src), (std::uint8_t*)dst);
    return 0;
}