#ifndef RB_TREE_HH
#define RB_TREE_HH

#include <functional>
#include <utility>

#include "rb_iterator.hh"
#include "rb_node.hh"

template<class T, class Cmp = std::less<T> >
class rb_tree {
    friend rb_test<T,Cmp>;
public:
    typedef rb_iterator<T,Cmp,false,false> iterator;
    typedef rb_iterator<T,Cmp,true ,false> const_iterator;
    typedef rb_iterator<T,Cmp,false,true > reverse_iterator;
    typedef rb_iterator<T,Cmp,true ,true > const_reverse_iterator;

private:
    static constexpr typename rb_node<T,Cmp>::color_type black = rb_node<T,Cmp>::black;
    static constexpr typename rb_node<T,Cmp>::color_type red = rb_node<T,Cmp>::red;

    rb_node<T,Cmp>* root_;
    size_t size_;

    void rotate_right(rb_node<T,Cmp>*);
    void rotate_left(rb_node<T,Cmp>*);
    void balance_at(rb_node<T,Cmp>*);

    rb_node<T,Cmp>* first_node();
    rb_node<T,Cmp>* last_node();
    const rb_node<T,Cmp>* first_node() const;
    const rb_node<T,Cmp>* last_node() const;

public:
    rb_tree() : root_(nullptr), size_(0) {}

    //TODO create a move version of insert
    iterator insert(const T& value);
    //iterator remove(const T& value);

    iterator begin() { return iterator(first_node()); }
    iterator end() { return iterator(nullptr); }
    const_iterator cbegin() const { return const_iterator(first_node()); }
    const_iterator cend() const { return const_iterator(nullptr); }
    reverse_iterator rbegin()  { return reverse_iterator(last_node()); }
    reverse_iterator rend() { return reverse_iterator(nullptr); }
    const_reverse_iterator crbegin() const  { return const_reverse_iterator(last_node()); }
    const_reverse_iterator crend() const { return const_reverse_iterator(nullptr); }
};

#include "rb_tree.tcc"

#endif