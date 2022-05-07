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

TEST(DynamicHuffman, decode_default_sequence)
{
    char buffer[buffer_size];
    char dst[sizeof(src)];
    std::memset(buffer, 0, buffer_size);
    std::size_t real_size = huffman::dynamic_huffman_encode((std::uint8_t*)src, sizeof(src), (std::uint8_t*)buffer);
    ASSERT_NO_THROW(huffman::dynamic_huffman_decode((std::uint8_t*)buffer, real_size, (std::uint8_t*)dst));
}
