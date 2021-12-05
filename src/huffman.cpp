#include <iostream>
#include <cstring>
#include <queue>
#include <vector>
#include <map>
#include <bitset>

#include "huffman.hpp"

namespace huffman {
///#################################
/// TODO size of base unit of coding
///#################################

class node {
public:
    node() = delete;
    node(std::size_t _weight, std::uint8_t _literal) : left(nullptr), right(nullptr), weight(_weight), literal(_literal) {}
    node(node* first, node* second) : weight(first->weight + second->weight), literal(0)
    {
        if (*first > *second) {
            left  = second;
            right = first;
        }
        else {
            left  = first;
            right = second;
        }
    }

    node* get_left()
    {
        return left;
    }

    node* get_right()
    {
        return right;
    }

    std::size_t get_weight()
    {
        return weight;
    }

    std::uint8_t get_literal()
    {
        return literal;
    }

    friend bool operator>(const node& item1, const node& item2)
    {
        return item1.weight > item2.weight;
    }

private:
    node* left;
    node* right;
    std::size_t weight;
    std::uint8_t literal;
};

void clear_tree(node* root)
{
    node* tmp_left  = root->get_left();
    node* tmp_right = root->get_right();
    delete root;
    if (tmp_left != nullptr)
        clear_tree(tmp_left);
    if (tmp_right != nullptr)
        clear_tree(tmp_right);
}

void get_statistic(const std::uint8_t* src, const std::size_t size, std::size_t* statistic)
{
    std::memset(statistic, 0, 256 * sizeof(std::size_t));
    for (std::size_t i = 0; i < size; ++i)
        statistic[src[i]]++;
}

void get_codes(node* root,
               std::map<std::uint8_t, std::vector<bool>>& codes,
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
    bool operator()(node* a, node* b) { return *a > *b; }
};

std::size_t huffman_encode(const std::uint8_t* src, const std::size_t size, std::uint8_t* dst)
{
    // for (std::size_t i = 0; i < size; ++i)
    //     std::cout << src[i];
    // std::cout << std::endl;

    // initialize tree leaf
    std::size_t st[256];
    get_statistic(src, size, st);
    //auto grater = [](node* a, node* b) mutable -> bool { return *a > *b; };
    std::priority_queue<node*, std::vector<node*>, greater> q;
    for (int i = 0; i < 256; ++i)
        if (st[i] > 0)
            q.push(new node(st[i], static_cast<uint8_t>(i)));

    // build tree
    while (q.size() != 1) {
        node* first = const_cast<node*>(q.top());
        q.pop();
        node* second = const_cast<node*>(q.top());
        q.pop();
        q.push(new node(first, second));
    }

    // tree root
    node* root = const_cast<node*>(q.top());
    q.pop();

    std::map<std::uint8_t, std::vector<bool>> codes;
    std::vector<bool> cache{};
    get_codes(root, codes, cache);

    // for (auto code : codes) {
    //     std::cout << "symbol - " << code.first << ", code - ";
    //     for (auto bit : code.second)
    //         std::cout << bit;
    //     std::cout << std::endl;
    // }

    std::size_t bit_ptr = 0;
    std::size_t byte_ptr = 0;
    for (std::size_t i = 0; i < size; ++i) {
        for (int j = codes[src[i]].size() - 1; j >= 0; --j) {
            if (bit_ptr >= 8) {
                bit_ptr = 0;
                byte_ptr++;
            }
            dst[byte_ptr] += (codes[src[i]][j] & 1u) << bit_ptr++;
        }
    }

    // for (std::size_t i = 0; i < byte_ptr + 1; ++i)
    //     std::cout << std::bitset<8>(dst[i]);
    // std::cout << std::endl;

    return byte_ptr + 1;
}

}