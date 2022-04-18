#include <cstring>
#include <vector>
#include <list>
#include <bitset>

#include "huffman.hpp"
#include "utils.hpp"

#define get_leaf_count(elems_count) ((elems_count) / 2 + 1)

namespace huffman {

using namespace detail;

inline void rebuild_tree(std::list<d_node<std::uint8_t>*>& root, d_node<std::uint8_t>* leaf) {
    ++(*leaf);
    for (d_node<std::uint8_t>* tmp = leaf->get_parent(); tmp != nullptr; tmp = tmp->get_parent())
        ++(*tmp);
    auto i_elem = root.begin();
    for (; i_elem != root.end(); ++i_elem)
        if ((*i_elem) == leaf)
            break;
    if (auto new_place = i_elem++; (*i_elem)->get_weight() > (*new_place)->get_weight()) {
        while ((*i_elem)->get_weight() > (*new_place)->get_weight())
            new_place++;
        std::swap(i_elem, new_place);
    }
}

std::size_t dynamic_huffman_encode(const std::uint8_t* src, const std::size_t src_size, std::uint8_t* dst)
{
    // list of leafs of huffman tree
    std::list<d_node<std::uint8_t>*> leafs;
    // push back empty element (weight == 0)
    leafs.push_back(new d_node<std::uint8_t>(0ul, 0));

    // now the root of tree is equal to an empty element
    std::list<d_node<std::uint8_t>*> root(leafs);

    // statistic
    std::size_t statistic[256];
    std::memset(statistic, 0, 256 * sizeof(std::size_t));

    for (std::size_t i = 0; i < src_size; ++i) {
        std::uint8_t literal = src[i];
        std::bitset<sizeof(std::uint32_t) * 8> code(0);
        d_node<std::uint8_t>* current_leaf = nullptr;
        // search for the right leaf of huffman tree
        for (auto i_elem = leafs.begin(); i_elem != leafs.end(); ++i_elem)
            if ((*i_elem)->get_literal() == literal && (*i_elem)->get_weight() > 0)
                current_leaf = *i_elem;

        if (statistic[literal] == 0) {
            code[0] = 0;        // remake
            code[1] = literal;  //
        }
        else {
            code = current_leaf->get_code();
        }
        dst[i] = code[0];
        rebuild_tree(root, current_leaf);
    }
    return 0;
}

}