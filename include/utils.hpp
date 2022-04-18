#pragma once

#include <map>
#include <vector>
#include <bitset>

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

    inline node<BaseUnitT>* get_left()
    {
        return left;
    }

    inline node<BaseUnitT>* get_right()
    {
        return right;
    }

    inline std::size_t get_weight()
    {
        return weight;
    }

    inline BaseUnitT get_literal()
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

protected:
    node<BaseUnitT>* left;
    node<BaseUnitT>* right;
    std::size_t weight;
    BaseUnitT literal;
};

template <typename BaseUnitT>
class d_node final : public node<BaseUnitT> {
public:
    d_node() = delete;
    d_node(const std::size_t _weight, const BaseUnitT _literal) : node<BaseUnitT>(_weight, _literal),
                                                      parent(nullptr),
                                                      code_size(1),
                                                      code(0) {}

    d_node(d_node<BaseUnitT>* first, d_node<BaseUnitT>* second) : node<BaseUnitT>(first, second),
                                                                  parent(nullptr),
                                                                  code_size(first->code_size + 1), // ????
                                                                  code(0) {}

    inline d_node<BaseUnitT>* get_parent()
    {
        return parent;
    }

    inline std::bitset<sizeof(std::uint32_t) * 8>& get_code()
    {
        return code;
    }

    void operator++()
    {
        this->weight++;
    }

private:
    d_node<BaseUnitT>* parent;
    std::uint32_t code_size;
    std::bitset<sizeof(std::uint32_t) * 8> code;
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

/* template <typename BaseUnitT>
struct tree_node {
    tree_node() : weight(0), code_size(1), code(0), literal(0) {}
    tree_node(std::size_t _weight, BaseUnitT _literal) : weight(_weight), code(0), literal(_literal) {}

    std::size_t weight;
    std::uint32_t code_size;
    std::bitset<sizeof(std::uint32_t) * 8> code;
    BaseUnitT literal;
};

template <typename BaseUnitT>
struct tree {
    tree() : current(nullptr), left(nullptr), right(nullptr), parent(nullptr) {}

    tree_node<BaseUnitT>* current;
    tree_node<BaseUnitT>* left;
    tree_node<BaseUnitT>* right;
    tree_node<BaseUnitT>* parent;

    static void clear_tree(tree<BaseUnitT>* root)
    {
        if (root != nullptr) {
            tree<BaseUnitT>* tmp_left  = root->left;
            tree<BaseUnitT>* tmp_right = root->right;
            delete root;
            if (tmp_left != nullptr)
                clear_tree(tmp_left);
            if (tmp_right != nullptr)
                clear_tree(tmp_right);
        }
    }
}; */

}
