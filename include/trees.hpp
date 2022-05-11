#pragma once

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

    inline node<BaseUnitT>* get_left() { return left; }

    inline node<BaseUnitT>* get_right() { return right; }

    inline std::size_t get_weight() { return weight; }

    inline BaseUnitT get_literal() { return literal; }

    friend bool operator>(const node<BaseUnitT>& item1, const node<BaseUnitT>& item2)
    {
        return item1.weight > item2.weight;
    }

    friend bool operator>=(const node<BaseUnitT>& item1, const node<BaseUnitT>& item2)
    {
        return item1.weight >= item2.weight;
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
                                                                  code_size(0),
                                                                  code(0) {}

    d_node(d_node<BaseUnitT>* first, d_node<BaseUnitT>* second) : node<BaseUnitT>(first, second),
                                                                  parent(nullptr),
                                                                  code_size(0),
                                                                  code(0) {}

    inline d_node<BaseUnitT>* get_parent() { return parent; }

    inline void set_parent(d_node<BaseUnitT>* _parent) { parent = _parent; }

    inline void set_left(d_node<BaseUnitT>* _left) { this->left = _left; }

    inline void set_right(d_node<BaseUnitT>* _right) { this->right = _right; }

    inline void set_code_size(std::uint32_t _code_size) { code_size = _code_size; }

    inline void set_code(const std::bitset<sizeof(std::uint32_t) * 8>& _code) { code = _code; }

    inline std::bitset<sizeof(std::uint32_t) * 8>& get_code() { return code; }

    inline std::uint32_t get_code_size() { return code_size; }

    void operator++() { this->weight++; }

private:
    d_node<BaseUnitT>* parent;
    std::uint32_t code_size;
    std::bitset<sizeof(std::uint32_t) * 8> code;
};

}
