#include <cstdint>

namespace huffman {

std::size_t huffman_encode(const std::uint8_t* src, const std::size_t src_size, std::uint8_t* dst);
std::size_t huffman_decode(const std::uint8_t* src, const std::size_t src_size, std::uint8_t* dst);

}
