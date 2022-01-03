#pragma once

#include <cstdint>

namespace lz77 {

constexpr std::size_t dictionary_size = 32768;

std::size_t lz77_encode(const std::uint8_t* src, const std::size_t src_size, std::uint8_t* dst);
std::size_t lz77_decode(const std::uint8_t* src, const std::size_t src_size, std::uint8_t* dst);

}
