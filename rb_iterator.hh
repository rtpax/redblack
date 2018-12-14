#ifndef RB_ITERATOR_HH
#define RB_ITERATOR_HH

#include <iterator>
#include <cstddef>

template <class T, class Cmp, class Alloc>
class rb_test;
template <class T, class Cmp, class Alloc>
class rb_node;
template <class T, class Cmp, class Alloc>
class rb_tree;

template<class T, class Cmp, class Alloc, bool is_const, bool is_reverse>
class rb_iterator {
    friend rb_test<T,Cmp,Alloc>;
    friend rb_tree<T,Cmp,Alloc>;
    friend rb_iterator<T,Cmp,Alloc,true,true>;
    friend rb_iterator<T,Cmp,Alloc,true,false>;
    friend rb_iterator<T,Cmp,Alloc,false,true>;
    friend rb_iterator<T,Cmp,Alloc,false,false>;
private:
    typedef rb_node<T,Cmp,Alloc> node_type;
    node_type* loc_;
public:
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;

    rb_iterator(rb_node<T,Cmp,Alloc>* loc) : loc_(loc) {}
    rb_iterator(std::enable_if<is_const, const rb_iterator<T,Cmp,Alloc,true,true>&> arg) : loc_(arg.loc_) {}
    rb_iterator(std::enable_if<is_const, const rb_iterator<T,Cmp,Alloc,true,false>&> arg) : loc_(arg.loc_) {}
    rb_iterator(const rb_iterator<T,Cmp,Alloc,false,true>& arg) : loc_(arg.loc_) {}
    rb_iterator(const rb_iterator<T,Cmp,Alloc,false,false>& arg) : loc_(arg.loc_) {}

    const T& operator*() const { return *loc_->elem; }

    bool operator==(const rb_iterator<T,Cmp,Alloc,true,true>& arg) { return loc_ == arg.loc_; }
    bool operator!=(const rb_iterator<T,Cmp,Alloc,true,true>& arg) { return loc_ != arg.loc_; }

    bool operator==(const rb_iterator<T,Cmp,Alloc,true,false>& arg) { return loc_ == arg.loc_; }
    bool operator!=(const rb_iterator<T,Cmp,Alloc,true,false>& arg) { return loc_ != arg.loc_; }

    bool operator==(const rb_iterator<T,Cmp,Alloc,false,true>& arg) { return loc_ == arg.loc_; }
    bool operator!=(const rb_iterator<T,Cmp,Alloc,false,true>& arg) { return loc_ != arg.loc_; }

    bool operator==(const rb_iterator<T,Cmp,Alloc,false,false>& arg) { return loc_ == arg.loc_; }
    bool operator!=(const rb_iterator<T,Cmp,Alloc,false,false>& arg) { return loc_ != arg.loc_; }

    rb_iterator operator++() {
        if(is_reverse)
            loc_ = loc_->sequential_prev();
        else
            loc_ = loc_->sequential_next();
        return *this;
    }
    rb_iterator operator--() {
        if(is_reverse)
            loc_ = loc_->sequential_next();
        else
            loc_ = loc_->sequential_prev();
        return *this;
    }
    rb_iterator operator++(int) {
        rb_iterator prev = *this;
        if(is_reverse)
            loc_ = loc_->sequential_prev();
        else
            loc_ = loc_->sequential_next();
        return prev;
    }
    rb_iterator operator--(int) {
        rb_iterator prev = *this;
        if(is_reverse)
            loc_ = loc_->sequential_next();
        else
            loc_ = loc_->sequential_prev();
        return prev;
    }
};

#endif