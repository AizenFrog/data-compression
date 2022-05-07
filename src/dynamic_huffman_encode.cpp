#include <cstring>
#include <list>
#include <bitset>

#include <iostream>

#include "huffman.hpp"
#include "utils.hpp"

namespace huffman {

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
    //for (auto a : root)
    //    std::cout << a->get_literal() << " " << a->get_weight() << std::endl;

    for (; i_elem != root.rend(); ++i_elem)
        if ((*i_elem) == leaf)
            break;

    while (true) {
        //if ((*i_elem)->get_weight() == 0)
        //    ++(*((*i_elem)->get_parent()));
        ++(*(*i_elem));
        //std::cout << "----" << (*i_elem)->get_literal() << " " << (*i_elem)->get_weight() << std::endl;
        if (*i_elem == root.front()) {
            //std::cout << "break" << std::endl;
            break;
        }
        else {
            // root of the tree is at the beginning,
            // so we need to use operator --
            auto new_place = i_elem;
            ++new_place;
            //std::cout << "new place - " << (*new_place)->get_literal() << " " << (*new_place)->get_weight() << std::endl;
            //std::cout << "i_elem - " << (*i_elem)->get_literal() << " " << (*i_elem)->get_weight() << std::endl;

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
                //std::cout << "parent - " << (*parent)->get_literal() << " " << (*parent)->get_weight() << std::endl;
                ++parent;
            }
            //std::cout << "parent - " << (*parent)->get_literal() << " " << (*parent)->get_weight() << std::endl;
            i_elem = parent;
        }
    }
    //set_node_code(root.front());
    //for (auto a : root)
    //    std::cout << a->get_literal() << " " << a->get_weight() << "; code size - " << a->get_code_size() << "; code: " << a->get_code() << std::endl;
}

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
        //std::cout << "literal - " << literal << "; leaf - " << current_leaf << std::endl;
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
            //std::cout << "code size - " << code_size << "; code: " << code << std::endl;
            rebuild_tree(root, new_leaf);
        }
        else {
            code      = current_leaf->get_code();
            code_size = current_leaf->get_code_size();
            rebuild_tree(root, current_leaf);
        }

        std::size_t n_code = code.to_ullong();
        for (std::uint32_t i = 0; i < code_size; ++i) {
            if (bit_index % 8 == 0) {
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