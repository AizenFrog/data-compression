#include <vector>
#include <list>
#include <cstdint>

#include "lz77.hpp"

namespace lz77 {

struct node {
    node(const std::uint16_t _offset, const std::uint16_t _length, const std::uint8_t _literal) :
        offset(_offset), length(_length), literal(_literal) {}
    std::uint16_t offset;
    std::uint16_t length;
    std::uint8_t literal;
};

std::size_t lz77_decode(const std::uint8_t* src, const std::size_t src_size, std::uint8_t* dst)
{
    std::list<node> decode_table;
    std::uint16_t offset = -1;
    std::uint16_t length = -1;
    std::uint8_t literal = 0;
    for (std::size_t src_ptr = 0; src_ptr < src_size;) {
        offset = *(reinterpret_cast<const std::uint16_t*>(src + src_ptr));
        src_ptr += sizeof(std::uint16_t);
        length = *(reinterpret_cast<const std::uint16_t*>(src + src_ptr));
        src_ptr += sizeof(std::uint16_t);
        literal = src[src_ptr++];
        decode_table.push_back(node(offset, length, literal));
    }

    std::size_t dst_ptr = 0;
    for (auto elem : decode_table) {
        if (elem.length > 0) {
            std::size_t start = dst_ptr - elem.offset;
            for (std::size_t i = 0; i < elem.length; ++i)
                dst[dst_ptr++] = dst[start + i];
        }
        dst[dst_ptr++] = elem.literal;
    }

    return dst_ptr;
}

}
