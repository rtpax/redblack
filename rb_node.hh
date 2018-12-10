#ifndef RB_NODE_HH
#define RB_NODE_HH

#include <utility>
#include <iostream>

template <class T, class Cmp>
class rb_test;
template <class T, class Cmp>
class rb_node;
template <class T, class Cmp, bool is_const, bool is_reverse>
class rb_iterator;

template<class T, class Cmp>
struct rb_node {
    friend rb_test<T,Cmp>;
    enum color_type { red = 0, black = 1 };

    color_type color;
    T elem;
    rb_node *left, *right, *parent;

    rb_node(T t, color_type c, rb_node* p) : color(c), elem(t), parent(p) {}

    bool is_root() { return parent == nullptr; }
    bool is_leaf() { return left == nullptr || right == nullptr; }

    bool is_right() { return parent->right == this; }
    bool is_left() { return parent->left == this; }

    rb_node* brother() { return parent->left == this ? parent->right : parent->left; }

    rb_node* sequential_next();
    rb_node* sequential_prev();

    std::pair<rb_node*,bool> unbalanced_insert(const T& value);

    void dump_tree(int depth = 0);
};

#include "rb_node.tcc"

#endif