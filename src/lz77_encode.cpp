#include <vector>
#include <list>
#include <cstdint>
#include <iostream>
#include <bitset>

#include "lz77.hpp"

namespace lz77 {

///#################################
/// TODO:
/// * size of base unit of coding
///
///#################################
struct node {
    node(const std::uint16_t _offset, const std::uint16_t _length, const std::uint8_t _literal) :
        offset(_offset), length(_length), literal(_literal) {}
    std::uint16_t offset;
    std::uint16_t length;
    std::uint8_t literal;
};

node find_matching(const std::uint8_t* src,
                   const std::size_t src_ptr,
                   const std::size_t dictionary_start,
                   const std::size_t dictionary_end)
{
    std::size_t offset = 0, length = 0;
    std::size_t dictionary_ptr = dictionary_start;
    std::size_t src_index = src_ptr;

    std::size_t start = -1;

    for (std::size_t i = dictionary_start; i < dictionary_end; ++i) {
        if (src[i] == src[src_ptr]) {
            dictionary_ptr = i + 1;
            src_index = src_ptr + 1;
            start = i;
            while (src[dictionary_ptr] == src[src_index] && dictionary_ptr != dictionary_end) {
                ++dictionary_ptr;
                ++src_index;
            }
            if (src_index - src_ptr >= length) {
                offset = dictionary_end - start;
                length = src_index - src_ptr;
            }
        }
    }
    return node(offset, length, src[src_ptr + length]);
}

void shift_dictionary(std::size_t& start, std::size_t& end, std::size_t length)
{
    end += length + 1;
    if (end - start > dictionary_size)
        start += end - dictionary_size;
}

std::size_t lz77_encode(const std::uint8_t* src, const std::size_t src_size, std::uint8_t* dst)
{
    std::size_t dictionary_start = 0;
    std::size_t dictionary_end   = 0;
    std::size_t src_ptr = 0;

    std::list<node> encode_table;

    while(src_ptr != src_size) {
        encode_table.push_back(find_matching(src, src_ptr, dictionary_start, dictionary_end));
        shift_dictionary(dictionary_start, dictionary_end, encode_table.back().length);
        src_ptr += encode_table.back().length + 1;
    }

    //for (auto elem : encode_table)
    //    std::cout << elem.offset << ", " << elem.length << ", " << std::bitset<8>(elem.literal) << std::endl;

    std::size_t dst_ptr = 0;
    for (auto elem : encode_table) {
        *(reinterpret_cast<std::uint16_t*>(dst + dst_ptr)) = elem.offset;
        dst_ptr += sizeof(std::uint16_t);
        *(reinterpret_cast<std::uint16_t*>(dst + dst_ptr)) = elem.length;
        dst_ptr += sizeof(std::uint16_t);
        dst[dst_ptr++] = elem.literal;
    }

    return dst_ptr;
}

}
