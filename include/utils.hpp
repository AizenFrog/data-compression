#pragma once

#include <map>
#include <vector>

namespace detail {

template <typename BaseUnitT>
class node {
public:
    node() = delete;
    node(std::size_t _weight, BaseUnitT _literal) : left(nullptr), right(nullptr), weight(_weight), literal(_literal) {}
    node(node<BaseUnitT>* first, node<BaseUnitT>* second) : weight(first->weight + second->weight), literal(0)
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

    node<BaseUnitT>* get_left()
    {
        return left;
    }

    node<BaseUnitT>* get_right()
    {
        return right;
    }

    std::size_t get_weight()
    {
        return weight;
    }

    BaseUnitT get_literal()
    {
        return literal;
    }

    friend bool operator>(const node<BaseUnitT>& item1, const node<BaseUnitT>& item2)
    {
        return item1.weight > item2.weight;
    }

    static void clear_tree(node<BaseUnitT>* root)
    {
        if (root != nullptr) {
            node<BaseUnitT>* tmp_left  = root->get_left();
            node<BaseUnitT>* tmp_right = root->get_right();
            delete root;
            if (tmp_left != nullptr)
                clear_tree(tmp_left);
            if (tmp_right != nullptr)
                clear_tree(tmp_right);
        }
    }

private:
    node<BaseUnitT>* left;
    node<BaseUnitT>* right;
    std::size_t weight;
    BaseUnitT literal;
};

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

}
