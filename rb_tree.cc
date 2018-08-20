
#include "rb_tree.hh"
#include <cassert>

template<class T, class Cmp> 
std::pair<rb_node<T,Cmp>*, bool>  rb_node<T,Cmp>::unbalanced_insert(T&& value) {
    Cmp cmp = Cmp();
    if(cmp(value, elem_)) {
        if(left_ == nullptr) {
            left_ = new rb_node<T,Cmp>(value, red);
            return std::pair<rb_node<T,Cmp>*, bool>(left_, true);
        } else {
            return left->unbalanced_insert(value, cmp);
        }
    } else if (cmp(elem_, value)) {
        if(right_ == nullptr) {
            right_ = new rb_node<T, Cmp>(value, red);
            return std::pair<rb_node<T,Cmp>*, bool>(right_, true);
        } else {
            return right->unbalanced_insert(value, cmp);
        }
    } else {
        return std::pair<rb_node<T,Cmp>*, bool>std::pair<rb_node<T,Cmp>*, bool>(nullptr, false);
    }
}

template<class T, class Cmp> void rb_tree<T,Cmp>::rotate(rb_node<T,Cmp>* pivot, rb_node::direction dir) {
    rb_node<T,Cmp>::* rot_dir = &rb_node<T,Cmp>::right_;
    rb_node<T,Cmp>::* counter_rot_dir = &rb_node<T,Cmp>::left_;

    if(dir == left) {
        rot_dir = &rb_node<T,Cmp>::left_;
        counter_rot_dir = &rb_node<T,Cmp>::right_;
    }

    pivot->*counter_rot_dir->parent_ = pivot->parent_;
    pivot->parent_ = pivot->*counter_rot_dir;

    pivot->*counter_rot_dir = pivot->parent_->*rot_dir;
    pivot->parent_->*rot_dir = pivot_;
    
    if(pivot->parent_->parent_ == nullptr) {
        root_ = pivot->parent_;
    } else {
        if(pivot->parent_->parent_->*rot_dir == pivot) {
            pivot->parent_->parent_->*rot_dir = pivot->parent_;
        } else {
            assert(pivot->parent_->parent_->*counter_rot_dir == pivot);
            pivot->parent_->parent_->*counter_rot_dir = pivot->parent_;
        }
    }
}

template<class T, class Cmp> rb_iterator<T,Cmp> rb_tree::insert(T&& value) {
    auto insertion = unbalance_insert(value);
    if(insertion.first == nullptr) {
        return end();
    }
    if(insertion.second == false) {
        return rb_iterator<T,Cmp>(this, insertion.first);
    }
}





//loosely based on https://www.cs.auckland.ac.nz/software/AlgAnim/red_black.html




