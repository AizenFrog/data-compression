#include <iostream>
#include <map>
#include <bitset>
#include <queue>

#include "huffman.hpp"
#include "utils.hpp"

namespace huffman {

std::size_t huffman_decode(const std::uint8_t* src, const std::size_t src_size, std::uint8_t* dst)
{
    using namespace detail;
    // initialize tree leaf
    std::priority_queue<node<std::uint8_t>*, std::vector<node<std::uint8_t>*>, greater> q;
    std::size_t src_index = 0;
    // barrier of statistic in output flow
    std::uint8_t barrier;
    bool is_first = true;

    while (true) {
        if (is_first) {
            barrier = *(src + src_index);
            is_first = false;
            q.push(new node<std::uint8_t>(*(reinterpret_cast<const std::size_t*>(src + src_index + 1)), src[src_index]));
            //std::cout << "literal - " << std::bitset<8>(src[src_index]) << ", count - " << *(reinterpret_cast<const std::size_t*>(src + src_index + 1)) <<std::endl;
            src_index += sizeof(std::uint8_t) + sizeof(std::size_t);
            continue;
        }
        if (src[src_index] == barrier) {
            ++src_index;
            break;
        }
        q.push(new node<std::uint8_t>(*(reinterpret_cast<const std::size_t*>(src + src_index + 1)), src[src_index]));
        //std::cout << "literal - " << std::bitset<8>(src[src_index]) << ", count - " << *(reinterpret_cast<const std::size_t*>(src + src_index + 1)) <<std::endl;
        src_index += sizeof(std::uint8_t) + sizeof(std::size_t);
    }

    // build tree
    while (q.size() != 1) {
        node<std::uint8_t>* first = const_cast<node<std::uint8_t>*>(q.top());
        q.pop();
        node<std::uint8_t>* second = const_cast<node<std::uint8_t>*>(q.top());
        q.pop();
        q.push(new node<std::uint8_t>(first, second));
    }

    // tree root
    node<std::uint8_t>* root = const_cast<node<std::uint8_t>*>(q.top());
    q.pop();

    // decode
    std::size_t bit_index = 0;
    std::size_t byte_index = src_index;
    node<std::uint8_t>* current_cell = root;
    std::size_t dst_index = 0;
    for (; byte_index < src_size;) {
        if (current_cell->get_left() == nullptr && current_cell->get_right() == nullptr) {
            dst[dst_index++] = current_cell->get_literal();
            current_cell = root;
            continue;
        }
        bool bit = src[byte_index] & (std::uint8_t)(1u << bit_index++);
        if (bit == 0)
            current_cell = current_cell->get_left();
        else if (bit == 1)
            current_cell = current_cell->get_right();
        if (bit_index >= 8) {
            bit_index = 0;
            byte_index++;
        }
    }

    node<std::uint8_t>::clear_tree(root);
    return dst_index;
}

}
