#include <cstring>
#include <list>
#include <bitset>

#include "huffman.hpp"
#include "utils.hpp"

namespace huffman {

#define IncrementCounters(src_index, bit_index) if (bit_index == 8) {\
                                                    bit_index = 0;\
                                                    ++src_index;\
                                                }

using namespace detail;

inline void set_node_code(d_node<std::uint8_t>* node)
{
    std::bitset<sizeof(std::uint32_t) * 8> p_code = node->get_code();
    std::uint32_t p_code_size = node->get_code_size();

    if (node->get_left() != nullptr) {
        p_code.set(p_code_size);
        reinterpret_cast<d_node<std::uint8_t>*>(node->get_left())->set_code(p_code);
        reinterpret_cast<d_node<std::uint8_t>*>(node->get_left())->set_code_size(p_code_size + 1);
        set_node_code(reinterpret_cast<d_node<std::uint8_t>*>(node->get_left()));
    }
    if (node->get_right() != nullptr) {
        p_code.reset(p_code_size);
        reinterpret_cast<d_node<std::uint8_t>*>(node->get_right())->set_code(p_code);
        reinterpret_cast<d_node<std::uint8_t>*>(node->get_right())->set_code_size(p_code_size + 1);
        set_node_code(reinterpret_cast<d_node<std::uint8_t>*>(node->get_right()));
    }
}

inline void swap_elements(std::list<d_node<std::uint8_t>*>::reverse_iterator& i_elem,
                          std::list<d_node<std::uint8_t>*>::reverse_iterator& new_place) {
    // std::cout << "swap - " << (*new_place)->get_literal() << " " << (*new_place)->get_weight() << std::endl;
    bool is_left_new_place = (*new_place)->get_parent()->get_left() == (*new_place) ? true : false;
    bool is_left_i_elem    = (*i_elem)->get_parent()->get_left() == (*i_elem) ? true : false;
    if (is_left_new_place)
        (*new_place)->get_parent()->set_left(*i_elem);
    else
        (*new_place)->get_parent()->set_right(*i_elem);
    if (is_left_i_elem)
        (*i_elem)->get_parent()->set_left(*new_place);
    else
        (*i_elem)->get_parent()->set_right(*new_place);
    d_node<std::uint8_t>* tmp = (*i_elem)->get_parent();
    (*i_elem)->set_parent((*new_place)->get_parent());
    (*new_place)->set_parent(tmp);
    std::swap(*i_elem, *new_place);
    std::swap(i_elem, new_place);
}

inline void rebuild_tree(std::list<d_node<std::uint8_t>*>& root, d_node<std::uint8_t>* leaf) {
    auto i_elem = root.rbegin();

    for (; i_elem != root.rend(); ++i_elem)
        if ((*i_elem) == leaf)
            break;

    while (true) {
        ++(*(*i_elem));
        if (*i_elem == root.front())
            break;
        else {
            // root of the tree is at the beginning,
            // so we need to use operator --
            auto new_place = i_elem;
            ++new_place;

            if (*new_place == (*i_elem)->get_parent()) {
                set_node_code((*i_elem)->get_parent());
                i_elem = new_place;
                continue;
            }

            if ((*i_elem)->get_weight() > (*new_place)->get_weight()) {
                auto tmp_place = new_place;
                ++tmp_place;
                while ((*i_elem)->get_weight() > (*tmp_place)->get_weight()) {
                    ++new_place;
                    ++tmp_place;
                }
                // swap parents and order in list
                swap_elements(i_elem, new_place);
            }
            set_node_code((*i_elem)->get_parent());
            auto parent = i_elem;
            ++parent;
            while (*parent != (*i_elem)->get_parent() && (*i_elem)->get_parent() != nullptr) {
                ++parent;
            }
            i_elem = parent;
        }
    }
}

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
    while (src_index != src_size) {
        d_node<std::uint8_t>* element = root.front();
        while(true) {
            if (element->get_left() == nullptr && element->get_right() == nullptr) {
                literal = 0;
                if (element->get_literal() == std::uint8_t(0) && element->get_weight() == 0) {
                    for (std::uint32_t i = 0; i < 8; ++i) {
                        literal += src[src_index] & (1 << bit_index++);
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