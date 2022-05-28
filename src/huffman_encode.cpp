#include <iostream>
#include <cstring>
#include <queue>
#include <vector>
#include <map>
#include <bitset>

#include "huffman.hpp"
#include "trees.hpp"
#include "utils.hpp"

namespace huffman {
///#################################
/// TODO:
/// * size of base unit of coding
/// * change vector with bitset
///
///#################################

void get_statistic(const std::uint8_t* src, const std::size_t size, std::size_t* statistic)
{
    std::memset(statistic, 0, 256 * sizeof(std::size_t));
    for (std::size_t i = 0; i < size; ++i)
        statistic[src[i]]++;
}

std::size_t huffman_encode(const std::uint8_t* src, const std::size_t src_size, std::uint8_t* dst)
{
    using namespace detail;
    // initialize tree leaf
    std::size_t st[256];
    get_statistic(src, src_size, st);
    //auto grater = [](node* a, node* b) mutable -> bool { return *a > *b; };
    std::priority_queue<node<std::uint8_t>*, std::vector<node<std::uint8_t>*>, greater> q;
    std::size_t dst_index = 0;
    // barrier of statistic in output flow
    std::uint8_t barrier = 0;
    bool is_first = true;

    for (int i = 0; i < 256; ++i)
        if (st[i] > 0) {
            if (is_first) {
                barrier = static_cast<std::uint8_t>(i);
                is_first = false;
            }
            *(dst + dst_index) = static_cast<std::uint8_t>(i);
            *(reinterpret_cast<std::size_t*>(dst + dst_index + 1)) = st[i];
            dst_index += sizeof(std::uint8_t) + sizeof(std::size_t);
            q.push(new node<std::uint8_t>(st[i], static_cast<std::uint8_t>(i)));
            //std::cout << "literal - " << (char)i << ", count - " << st[i] <<std::endl;
        }
    dst[dst_index++] = barrier;

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

    std::map<std::uint8_t, std::vector<bool>> codes;
    std::vector<bool> cache{};
    get_codes(root, codes, cache);

    std::size_t bit_ptr = 0;
    std::size_t byte_ptr = dst_index;
    for (std::size_t i = 0; i < src_size; ++i) {
        for (int j = 0; j < codes[src[i]].size(); ++j) {
            if (bit_ptr >= 8) {
                bit_ptr = 0;
                byte_ptr++;
            }
            dst[byte_ptr] += (codes[src[i]][j] & 1u) << bit_ptr++;
        }
    }

    node<std::uint8_t>::clear_tree(root);
    return byte_ptr + 1;
}

}
