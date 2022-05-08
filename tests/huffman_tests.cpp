#include <cstring>

#include "gtest/gtest.h"
#include "huffman.hpp"
#include "details/constants.hpp"

TEST(Huffman, encode_default_sequence)
{
    char buffer[buffer_size];
    std::memset(buffer, 0, buffer_size);
    ASSERT_NO_THROW(huffman::huffman_encode((std::uint8_t*)src, sizeof(src), (std::uint8_t*)buffer));
}

TEST(Huffman, decode_defauld_sequence)
{
    char buffer[buffer_size];
    char dst[sizeof(src)];
    std::memset(buffer, 0, buffer_size);
    std::size_t real_size = huffman::huffman_encode((std::uint8_t*)src, sizeof(src), (std::uint8_t*)buffer);
    ASSERT_NO_THROW(huffman::huffman_decode((std::uint8_t*)buffer, real_size, (std::uint8_t*)dst));
}

TEST(Huffman, equal_src_dst_sizes)
{
    char buffer[buffer_size];
    char dst[sizeof(src)];
    std::memset(buffer, 0, buffer_size);
    std::size_t real_size = huffman::huffman_encode((std::uint8_t*)src, sizeof(src), (std::uint8_t*)buffer);
    std::size_t decompressed_size = huffman::huffman_decode((std::uint8_t*)buffer, real_size, (std::uint8_t*)dst);
    EXPECT_EQ(sizeof(src), decompressed_size);
}

TEST(Huffman, equal_src_dst_source)
{
    char buffer[buffer_size];
    char dst[sizeof(src)];
    std::memset(buffer, 0, buffer_size);
    std::size_t real_size = huffman::huffman_encode((std::uint8_t*)src, sizeof(src), (std::uint8_t*)buffer);
    std::size_t decompressed_size = huffman::huffman_decode((std::uint8_t*)buffer, real_size, (std::uint8_t*)dst);
    for (std::size_t i = 0; i < sizeof(src); ++i)
        EXPECT_EQ(src[i], dst[i]);
}
