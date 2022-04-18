#include <cstring>

#include "gtest/gtest.h"
#include "huffman.hpp"
#include "details/constants.hpp"

TEST(DynamicHuffman, encode_default_sequence)
{
    char buffer[buffer_size];
    std::memset(buffer, 0, buffer_size);
    ASSERT_NO_THROW(huffman::dynamic_huffman_encode((std::uint8_t*)src, sizeof(src), (std::uint8_t*)buffer));
}