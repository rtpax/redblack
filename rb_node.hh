#ifndef RB_NODE_HH
#define RB_NODE_HH

#include <utility>

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

    bool is_root() { return parent == nullptr; }
    bool is_leaf() { return left == nullptr || right == nullptr; }
    rb_node* brother() { return parent->left == this ? parent->right : parent->left; }
    std::pair<rb_node*,bool> unbalanced_insert(T&& value);
};

#endif