#pragma once

#include <cstdint>

namespace huffman {

// static huffman
std::size_t huffman_encode(const std::uint8_t* src, const std::size_t src_size, std::uint8_t* dst);
std::size_t huffman_decode(const std::uint8_t* src, const std::size_t src_size, std::uint8_t* dst);

// dynamic huffman
std::size_t dynamic_huffman_encode(const std::uint8_t* src, const std::size_t src_size, std::uint8_t* dst);
std::size_t dynamic_huffman_decode(const std::uint8_t* src, const std::size_t src_size, std::uint8_t* dst);

}
