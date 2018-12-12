#ifndef RB_TREE_HH
#define RB_TREE_HH

#include <functional>
#include <utility>
#include <memory>

#include "rb_iterator.hh"
#include "rb_node.hh"

template<class T, class Cmp = std::less<T>>
class rb_tree {
    friend rb_test<T,Cmp>;
    friend int main(int,char**);
public:
    typedef rb_iterator<T,Cmp,false,false> iterator;
    typedef rb_iterator<T,Cmp,true ,false> const_iterator;
    typedef rb_iterator<T,Cmp,false,true > reverse_iterator;
    typedef rb_iterator<T,Cmp,true ,true > const_reverse_iterator;

private:
    static constexpr typename rb_node<T,Cmp>::color_type black = rb_node<T,Cmp>::black;
    static constexpr typename rb_node<T,Cmp>::color_type red = rb_node<T,Cmp>::red;

    rb_node<T,Cmp>* rend_;
    rb_node<T,Cmp>* end_;
    rb_node<T,Cmp>* root_;
    size_t size_;

    void rotate_right(rb_node<T,Cmp>*);
    void rotate_left(rb_node<T,Cmp>*);

    void balance_insertion(rb_node<T,Cmp>*);
    void balance_right_deletion(rb_node<T,Cmp>*);
    void balance_left_deletion(rb_node<T,Cmp>*);
    void treat_as_delete(rb_node<T,Cmp>*);

    rb_node<T,Cmp>* unbalanced_delete(rb_node<T,Cmp>*);
    void swap_nodes(rb_node<T,Cmp>*,rb_node<T,Cmp>*);
public:
    rb_tree() : rend_(new rb_node<T,Cmp>(nullptr, black, nullptr)),
            end_(new rb_node<T,Cmp>(nullptr, red, rend_)), 
            root_(rend_), size_(0) {
        rend_->right = end_;
    }

    iterator insert(const T& value);
    bool contains(const T& value) const;
    iterator find(const T& value);
    const_iterator find(const T& value) const;
    iterator erase(iterator);

    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    iterator begin() { return ++iterator(rend_); }
    iterator end() { return iterator(end_); }
    const_iterator cbegin() const { return ++const_iterator(rend_); }
    const_iterator cend() const { return const_iterator(end_); }
    reverse_iterator rbegin()  { return ++reverse_iterator(end_); }
    reverse_iterator rend() { return reverse_iterator(rend_); }
    const_reverse_iterator crbegin() const  { return ++const_reverse_iterator(end_); }
    const_reverse_iterator crend() const { return const_reverse_iterator(rend_); }
};

#include "rb_tree.tcc"

#endif