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

TEST(DynamicHuffman, equal_src_dst_sizes)
{
    char buffer[buffer_size];
    char dst[sizeof(src)];
    std::memset(buffer, 0, buffer_size);
    std::size_t real_size = huffman::dynamic_huffman_encode((std::uint8_t*)src, sizeof(src), (std::uint8_t*)buffer);
    std::size_t decompressed_size = huffman::dynamic_huffman_decode((std::uint8_t*)buffer, real_size, (std::uint8_t*)dst);
    EXPECT_EQ(sizeof(src), decompressed_size);
}

TEST(DynamicHuffman, equal_src_dst_source)
{
    char buffer[buffer_size];
    char dst[sizeof(src)];
    std::memset(buffer, 0, buffer_size);
    std::size_t real_size = huffman::dynamic_huffman_encode((std::uint8_t*)src, sizeof(src), (std::uint8_t*)buffer);
    std::size_t decompressed_size = huffman::dynamic_huffman_decode((std::uint8_t*)buffer, real_size, (std::uint8_t*)dst);
    for (std::size_t i = 0; i < sizeof(src); ++i)
        EXPECT_EQ(src[i], dst[i]);
}

TEST(DynamicHuffman, extreme_tree_from_src)
{
    // src_size will be 32896 bytes
    std::size_t src_size = 0;
    for (int i = 1; i <= 256; ++i)
        src_size += i;
    std::uint8_t* test_src = new std::uint8_t[src_size];
    std::size_t count = 0;
    for (std::size_t i = 0; i < 256 && count < src_size; ++i)
        for (std::size_t j = 0; j <= i && count < src_size; ++j)
            test_src[count++] = (std::uint8_t)i;
    // buffer size will be 34185
    int buf_size = 34200;
    std::uint8_t* buffer = new std::uint8_t[buf_size];
    std::uint8_t* dst    = new std::uint8_t[src_size];
    std::memset(buffer, 0, buffer_size);
    std::size_t real_size = huffman::dynamic_huffman_encode((std::uint8_t*)test_src, src_size, (std::uint8_t*)buffer);
    std::size_t decompressed_size = huffman::dynamic_huffman_decode((std::uint8_t*)buffer, real_size, (std::uint8_t*)dst);
    EXPECT_EQ(src_size, decompressed_size);
    for (std::size_t i = 0; i < src_size; ++i)
        EXPECT_EQ(test_src[i], dst[i]);
}
