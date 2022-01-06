#include <cstring>

#include "gtest/gtest.h"
#include "lz77.hpp"
#include "details/constants.hpp"

TEST(LZ77, encode_default_sequence)
{
    char buffer[buffer_size];
    std::memset(buffer, 0, buffer_size);
    ASSERT_NO_THROW(lz77::lz77_encode((const std::uint8_t*)src, sizeof(src), (std::uint8_t*)buffer));
}

TEST(LZ77, decode_defauld_sequence)
{
    char buffer[buffer_size];
    char dst[sizeof(src)];
    std::memset(buffer, 0, buffer_size);
    std::size_t real_size = lz77::lz77_encode((const std::uint8_t*)src, sizeof(src), (std::uint8_t*)buffer);
    ASSERT_NO_THROW(lz77::lz77_decode((std::uint8_t*)buffer, real_size, (std::uint8_t*)dst));
}

TEST(LZ77, equal_src_dst_sizes)
{
    char buffer[buffer_size];
    char dst[sizeof(src)];
    std::memset(buffer, 0, buffer_size);
    std::size_t real_size = lz77::lz77_encode((const std::uint8_t*)src, sizeof(src), (std::uint8_t*)buffer);
    std::size_t decompressed_size = lz77::lz77_decode((std::uint8_t*)buffer, real_size, (std::uint8_t*)dst);
    EXPECT_EQ(sizeof(src), decompressed_size);
}

TEST(LZ77, equal_src_dst_source)
{
    char buffer[buffer_size];
    char dst[sizeof(src)];
    std::memset(buffer, 0, buffer_size);
    std::size_t real_size = lz77::lz77_encode((const std::uint8_t*)src, sizeof(src), (std::uint8_t*)buffer);
    std::size_t decompressed_size = lz77::lz77_decode((std::uint8_t*)buffer, real_size, (std::uint8_t*)dst);
    for (std::size_t i = 0; i < sizeof(src); ++i)
        EXPECT_EQ(src[i], dst[i]);
}
