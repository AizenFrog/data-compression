#include <cstring>

#include "gtest/gtest.h"
#include "huffman.hpp"

constexpr int buffer_size = 100;

TEST(Huffman, encode_default_sequence)
{
    char word[] = { "beep boop beer!" };
    char buffer[buffer_size];
    std::memset(buffer, 0, buffer_size);
    ASSERT_NO_THROW(huffman::huffman_encode((std::uint8_t*)word, sizeof(word), (std::uint8_t*)buffer));
}


