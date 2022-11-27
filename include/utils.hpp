#pragma once

#include <map>
#include <vector>
#include <list>
#include <bitset>

#include "trees.hpp"

namespace detail {

inline void* allocate_memory(const std::size_t size)
{
    return malloc(size);
}

inline void deallocate_memory(void* ptr)
{
    free(ptr);
}

template <typename BaseUnitT>
void get_codes(node<BaseUnitT>* root,
               std::map<BaseUnitT, std::vector<bool>>& codes,
               std::vector<bool>& cache)
{
    if (root->get_left() != nullptr) {
        cache.push_back(0);
        get_codes(root->get_left(), codes, cache);
        cache.pop_back();
    }
    if (root->get_right() != nullptr) {
        cache.push_back(1);
        get_codes(root->get_right(), codes, cache);
        cache.pop_back();
    }
    if (root->get_left() == nullptr && root->get_right() == nullptr)
        codes[root->get_literal()] = cache;
}

class greater {
public:
    template <typename BaseUnitT>
    bool operator()(node<BaseUnitT>* a, node<BaseUnitT>* b) { return *a > *b; }
};

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

        if (*i_elem == root.front()) {
            // calling this function just once (on some file this worse than it multiple times times)
            // set_node_code(*i_elem);
            break;
        }

        else {
            // root of the tree is at the beginning,
            // so we need to use operator --
            auto new_place = i_elem;
            ++new_place;

            // 1st adding weight to leaf and adding weight to single leaf in tree
            if (*new_place == (*i_elem)->get_parent() && ((*i_elem)->get_weight() <= 1 || (*new_place)->get_parent() == nullptr)) {
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
                if ((*i_elem)->get_parent() != *new_place)
                    swap_elements(i_elem, new_place);
            }
            set_node_code((*i_elem)->get_parent());
            auto parent = i_elem;
            ++parent;
            while (*parent != (*i_elem)->get_parent() && (*i_elem)->get_parent() != nullptr)
                ++parent;
            i_elem = parent;
        }
    }
}

}
