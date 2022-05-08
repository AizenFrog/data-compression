#include <cstring>
#include <list>
#include <bitset>

#include "huffman.hpp"
#include "trees.hpp"
#include "utils.hpp"

namespace huffman {

using namespace detail;

std::size_t dynamic_huffman_encode(const std::uint8_t* src, const std::size_t src_size, std::uint8_t* dst)
{
    // list of leafs of huffman tree
    std::list<d_node<std::uint8_t>*> leafs;
    // push back empty element (weight == 0)
    leafs.push_back(new d_node<std::uint8_t>(0ul, 0));

    // now the root of tree is equal to an empty element
    std::list<d_node<std::uint8_t>*> root(leafs);

    std::size_t   real_size = 0;
    std::uint32_t bit_index = 0;
    std::uint8_t  tmp       = 0;

    for (std::size_t i = 0; i < src_size; ++i) {
        std::uint8_t literal = src[i];
        std::bitset<sizeof(std::uint32_t) * 8> code(0);
        std::uint32_t code_size = 0;
        d_node<std::uint8_t>* current_leaf = nullptr;
        // search for the right leaf of huffman tree
        for (auto i_elem = leafs.begin(); i_elem != leafs.end(); ++i_elem)
            if ((*i_elem)->get_literal() == literal && (*i_elem)->get_weight() > 0) {
                current_leaf = *i_elem;
                break;
            }

        if (current_leaf == nullptr) {
            d_node<std::uint8_t>* zero_node = root.back();
            d_node<std::uint8_t>* zero_node_parent = zero_node->get_parent();
            d_node<std::uint8_t>* new_leaf  = new d_node<std::uint8_t>(0ul, literal);
            d_node<std::uint8_t>* new_node  = new d_node<std::uint8_t>(new_leaf, zero_node);
            new_leaf->set_parent(new_node);
            zero_node->set_parent(new_node);
            new_node->set_parent(zero_node_parent);
            leafs.push_back(new_leaf);

            root.pop_back();
            root.push_back(new_node);
            root.push_back(new_leaf);
            root.push_back(zero_node);

            if (zero_node_parent != nullptr)
                zero_node_parent->set_right(new_node);

            code |= new_leaf->get_literal();
            code_size = zero_node->get_code_size();
            code <<= code_size;
            code |= zero_node->get_code();
            code_size += 8;
            rebuild_tree(root, new_leaf);
        }
        else {
            code      = current_leaf->get_code();
            code_size = current_leaf->get_code_size();
            rebuild_tree(root, current_leaf);
        }

        std::size_t n_code = code.to_ullong();
        for (std::uint32_t i = 0; i < code_size; ++i) {
            if (bit_index == 8) {
                dst[real_size++] = tmp;
                tmp = bit_index = 0;
            }
            tmp += (std::uint8_t)(n_code & 1ul) << bit_index++;
            n_code >>= 1;
        }
        dst[real_size] += tmp;
    }

    d_node<std::uint8_t>::clear_tree(root.front());
    return real_size;
}

}