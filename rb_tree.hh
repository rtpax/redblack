#ifndef RB_TREE_HH
#define RB_TREE_HH

#include <functional>
#include <utility>
#include <memory>
#include <limits>

#include "rb_iterator.hh"
#include "rb_node.hh"

template<class T, class Cmp = std::less<T>, class Alloc = std::allocator<T>>
class rb_tree {
    friend rb_test<T,Cmp,Alloc>;
    friend int main(int,char**);
public:
    typedef rb_iterator<T,Cmp,Alloc,false,false> iterator;
    typedef rb_iterator<T,Cmp,Alloc,true ,false> const_iterator;
    typedef rb_iterator<T,Cmp,Alloc,false,true > reverse_iterator;
    typedef rb_iterator<T,Cmp,Alloc,true ,true > const_reverse_iterator;

private:
    static constexpr typename rb_node<T,Cmp,Alloc>::color_type black = rb_node<T,Cmp,Alloc>::black;
    static constexpr typename rb_node<T,Cmp,Alloc>::color_type red = rb_node<T,Cmp,Alloc>::red;

    Cmp cmp_;
    Alloc alloc_;

    rb_node<T,Cmp,Alloc>* rend_;
    rb_node<T,Cmp,Alloc>* end_;
    rb_node<T,Cmp,Alloc>* root_;
    size_t size_;

    void rotate_right(rb_node<T,Cmp,Alloc>*);
    void rotate_left(rb_node<T,Cmp,Alloc>*);

    void balance_insertion(rb_node<T,Cmp,Alloc>*);
    void balance_right_deletion(rb_node<T,Cmp,Alloc>*);
    void balance_left_deletion(rb_node<T,Cmp,Alloc>*);
    void treat_as_delete(rb_node<T,Cmp,Alloc>*);

    rb_node<T,Cmp,Alloc>* unbalanced_delete(rb_node<T,Cmp,Alloc>*);
    void swap_nodes(rb_node<T,Cmp,Alloc>*,rb_node<T,Cmp,Alloc>*);
public:
    rb_tree() : cmp_(), alloc_(),
            rend_(new rb_node<T,Cmp,Alloc>(nullptr, black, nullptr)),
            end_(new rb_node<T,Cmp,Alloc>(nullptr, red, rend_)), 
            root_(rend_), size_(0) {
        rend_->right = end_;
    }
    rb_tree(const Cmp& cmp, const Alloc& alloc = Alloc()) : 
            cmp_(cmp), alloc_(alloc),
            rend_(new rb_node<T,Cmp,Alloc>(nullptr, black, nullptr)),
            end_(new rb_node<T,Cmp,Alloc>(nullptr, red, rend_)), 
            root_(rend_), size_(0) {
        rend_->right = end_;
    }
    rb_tree(const Alloc& alloc) : rb_tree(Cmp(), alloc) {}
    rb_tree(std::initializer_list<T> init, 
            const Cmp& cmp = Cmp(), const Alloc& alloc = Alloc()) : 
            rb_tree(cmp, alloc) {
        for(const T& t : init) {
            insert(t);
        }
    }
    template <class It>
    rb_tree(It first, It last, Cmp cmp = Cmp()) : rb_tree() {
        while(first != last) {
            insert(*first);
            ++last;
        }
    }
    rb_tree(const rb_tree& copy, Alloc alloc) : rb_tree(alloc) {
        for(const T& t : copy) {
            insert(t);
        }
    }
    rb_tree(const rb_tree& copy) : rb_tree(copy, Alloc()) {}
    rb_tree(rb_tree&& steal) {
        cmp_ = steal.cmp_;
        alloc_ = steal.alloc_;
        root_ = steal.root_;
        root_ = steal.end_;
        root_ = steal.rend_;
        size_ = steal.size_;

        steal.rend_ = new rb_node<T,Cmp,Alloc>(nullptr, black, nullptr);
        steal.end_ = new rb_node<T,Cmp,Alloc>(nullptr, red, rend_);
        steal.root_ = rend_;
        steal.size_ = 0;
        steal.rend_->right = steal.end_;
    }
    rb_tree(rb_tree&& steal, Alloc alloc) : rb_tree(steal, alloc) {}


    iterator insert(const T& value);
    bool contains(const T& value) const;
    iterator find(const T& value);
    const_iterator find(const T& value) const;
    iterator erase(iterator);

    size_t max_size() const { return std::numeric_limits<size_t>::max; }
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