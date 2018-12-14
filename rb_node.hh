#ifndef RB_NODE_HH
#define RB_NODE_HH

#include <utility>
#include <iostream>

template <class T, class Cmp, class Alloc>
class rb_test;
template <class T, class Cmp, class Alloc>
class rb_node;
template <class T, class Cmp, class Alloc, bool is_const, bool is_reverse>
class rb_iterator;

template<class T, class Cmp, class Alloc>
struct rb_node {
    friend rb_test<T,Cmp,Alloc>;
    enum color_type { red, black };

    color_type color;
    const T* elem;
    rb_node *left, *right, *parent;

    rb_node(T* t, color_type c, rb_node* p) : color(c), elem(t), left(nullptr), right(nullptr), parent(p) {}
    rb_node() : color(red), elem(nullptr), left(nullptr), right(nullptr), parent(nullptr) {}

    bool is_root() const { return parent == nullptr; }

    bool is_right() const { return parent->right == this; }
    bool is_left() const { return parent->left == this; }

    bool has_right() const { return right != nullptr; }
    bool has_left() const { return left != nullptr; }

    bool is_red() const { return color == red; }
    bool is_black() const { return color == black; }

    bool has_elem() const { return elem != nullptr; }

    bool is_end() const { 
        return !has_elem() && (is_root() ? has_left() : is_right()); 
    }
    bool is_rend() const { 
        return !has_elem() && (is_root() ? has_right() : is_left()); 
    }

    rb_node* brother() { return parent->left == this ? parent->right : parent->left; }

    rb_node* sequential_next();
    rb_node* sequential_prev();
    
    void dump_tree(int depth = 0) const;
    void dump_node() const;
};

#include "rb_node.tcc"

#endif