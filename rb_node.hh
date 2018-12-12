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
    enum color_type { red, black };

    color_type color;
    const T* elem;
    rb_node *left, *right, *parent;

    rb_node(T t, color_type c, rb_node* p) : color(c), elem(new T(t)), parent(p) {}
    rb_node(T* t, color_type c, rb_node* p) : color(c), elem(t), parent(p) {}
    rb_node() {}
    ~rb_node() { delete elem; }

    bool is_root() { return parent == nullptr; }
    bool is_leaf() { return left == nullptr || right == nullptr; }

    bool is_right() { return parent->right == this; }
    bool is_left() { return parent->left == this; }

    bool has_right() { return right != nullptr; }
    bool has_left() { return left != nullptr; }

    bool has_elem() { return elem != nullptr; }

    bool is_end() { 
        return !has_elem() && (is_root() ? has_left() : is_right()); 
    }
    bool is_rend() { 
        return !has_elem() && (is_root() ? has_right() : is_left()); 
    }

    rb_node* brother() { return parent->left == this ? parent->right : parent->left; }

    rb_node* sequential_next();
    rb_node* sequential_prev();

    std::pair<rb_node*,bool> unbalanced_insert(const T& value);
    
    void dump_tree(int depth = 0);
    void dump_node();
};

#include "rb_node.tcc"

#endif