#ifndef RB_TREE_HH
#define RB_TREE_HH

#include <functional>
#include <utility>

#include "rb_iterator.hh"

template <class T, class Cmp>
class rb_test;
template <class T, class Cmp>
class rb_node;
template <class T, class Cmp, bool is_const, bool is_reverse>
class rb_iterator;


template<class T, class Cmp = std::less<T> >
class rb_tree {
    friend rb_test<T,Cmp>;
public:
    typedef rb_iterator<T,Cmp,false,false> iterator;
    typedef rb_iterator<T,Cmp,true ,false> const_iterator;
    typedef rb_iterator<T,Cmp,false,true > reverse_iterator;
    typedef rb_iterator<T,Cmp,true ,true > const_reverse_iterator;
    
    enum direction { left = 0, right = 1 };

private:
    rb_node<T,Cmp>* root_;
    size_t size_;

    void rotate_right(rb_node<T,Cmp>*);
    void rotate_left(rb_node<T,Cmp>*);
    void balance_at(rb_node<T,Cmp>*);

public:
    rb_tree();

    iterator insert(const T& value) { return insert(std::move(T(value))); }
    iterator insert(T&& value);
    iterator remove(const T& value);

    iterator begin();
    iterator end();
    const_iterator cbegin() const;
    const_iterator cend() const;
    reverse_iterator rbegin();
    reverse_iterator rend();
    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;            
};

#endif