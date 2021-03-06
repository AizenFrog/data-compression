#include <cstring>
#include <list>
#include <bitset>

#include "huffman.hpp"
#include "trees.hpp"
#include "utils.hpp"

namespace huffman {

#define IncrementCounters(src_index, bit_index) if (bit_index == 8) {\
                                                    bit_index = 0;\
                                                    ++src_index;\
                                                }

using namespace detail;

std::size_t dynamic_huffman_decode(const std::uint8_t* src, const std::size_t src_size, std::uint8_t* dst)
{
    std::size_t  real_size = 0;
    std::size_t  src_index = 0;
    std::uint8_t literal = src[src_index++];
    dst[real_size++] = literal;
    d_node<std::uint8_t>* zero_leaf  = new d_node<std::uint8_t>(0ul, 0);
    d_node<std::uint8_t>* first_leaf = new d_node<std::uint8_t>(0ul, literal);
    d_node<std::uint8_t>* first_node = new d_node<std::uint8_t>(first_leaf, zero_leaf);
    zero_leaf->set_parent(first_node);
    first_leaf->set_parent(first_node);

    // now the root of tree is equal to an empty element
    std::list<d_node<std::uint8_t>*> root;

    root.push_back(first_node);
    root.push_back(first_leaf);
    root.push_back(zero_leaf);
    rebuild_tree(root, first_leaf);

    std::uint32_t bit_index = 0;
    while (src_index < src_size) {
        d_node<std::uint8_t>* element = root.front();
        while(true) {
            if (element->get_left() == nullptr && element->get_right() == nullptr) {
                literal = 0;
                if (element->get_literal() == std::uint8_t(0) && element->get_weight() == 0) {
                    for (std::uint32_t i = 0; i < 8; ++i) {
                        literal += ((src[src_index] & (1 << bit_index)) >> bit_index) << i;
                        ++bit_index;
                        IncrementCounters(src_index, bit_index);
                    }
                    d_node<std::uint8_t>* zero_node = root.back();
                    d_node<std::uint8_t>* zero_node_parent = zero_node->get_parent();
                    d_node<std::uint8_t>* new_leaf  = new d_node<std::uint8_t>(0ul, literal);
                    d_node<std::uint8_t>* new_node  = new d_node<std::uint8_t>(new_leaf, zero_node);
                    new_leaf->set_parent(new_node);
                    zero_node->set_parent(new_node);
                    new_node->set_parent(zero_node_parent);

                    root.pop_back();
                    root.push_back(new_node);
                    root.push_back(new_leaf);
                    root.push_back(zero_node);

                    if (zero_node_parent != nullptr)
                        zero_node_parent->set_right(new_node);

                    rebuild_tree(root, new_leaf);
                }
                else {
                    literal = element->get_literal();
                    rebuild_tree(root, element);
                }
                dst[real_size++] = literal;
                break;
            }
            bool bit = src[src_index] & (1 << bit_index++);
            IncrementCounters(src_index, bit_index);
            if (bit == 1)
                element = reinterpret_cast<d_node<std::uint8_t>*>(element->get_left());
            else
                element = reinterpret_cast<d_node<std::uint8_t>*>(element->get_right());
        }
    }

    d_node<std::uint8_t>::clear_tree(root.front());
    return real_size;
}

}
