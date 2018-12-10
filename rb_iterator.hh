#ifndef RB_ITERATOR_HH
#define RB_ITERATOR_HH

template <class T, class Cmp>
class rb_test;
template <class T, class Cmp>
class rb_node;

template<class T, class Cmp, bool is_const, bool is_reverse>
class rb_iterator {
    friend rb_test<T,Cmp>;
    friend rb_iterator<T,Cmp,true,true>;
    friend rb_iterator<T,Cmp,true,false>;
    friend rb_iterator<T,Cmp,false,true>;
    friend rb_iterator<T,Cmp,false,false>;
private:
    typedef rb_node<T,Cmp> node_type;
    node_type* loc_;
public:
    rb_iterator(rb_node<T,Cmp>* loc) : loc_(loc) {}

    const T& operator*() const { return loc_->elem; }

    bool operator==(rb_iterator<T,Cmp,true,true>& arg) { return loc_ == arg.loc_; }
    bool operator!=(rb_iterator<T,Cmp,true,true>& arg) { return loc_ != arg.loc_; }

    bool operator==(rb_iterator<T,Cmp,true,false>& arg) { return loc_ == arg.loc_; }
    bool operator!=(rb_iterator<T,Cmp,true,false>& arg) { return loc_ != arg.loc_; }

    bool operator==(rb_iterator<T,Cmp,false,true>& arg) { return loc_ == arg.loc_; }
    bool operator!=(rb_iterator<T,Cmp,false,true>& arg) { return loc_ != arg.loc_; }

    bool operator==(rb_iterator<T,Cmp,false,false>& arg) { return loc_ == arg.loc_; }
    bool operator!=(rb_iterator<T,Cmp,false,false>& arg) { return loc_ != arg.loc_; }

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