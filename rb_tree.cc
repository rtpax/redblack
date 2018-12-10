
#include "rb_tree.hh"
#include <cassert>

template<class T, class Cmp> 
void rb_tree<T,Cmp>::rotate_right(rb_node<T,Cmp>* pivot) {
    pivot->left->parent = pivot->parent;
    pivot->parent = pivot->left;

    pivot->left = pivot->parent->right;
    pivot->parent->right = pivot;
    
    if(pivot->parent->parent == nullptr) {
        root_ = pivot->parent;
    } else {
        if(pivot->parent->parent->right == pivot) {
            pivot->parent->parent->right = pivot->parent;
        } else {
            assert(pivot->parent->parent->left == pivot);
            pivot->parent->parent->left = pivot->parent;
        }
    }
}

template<class T, class Cmp> 
void rb_tree<T,Cmp>::rotate_left(rb_node<T,Cmp>* pivot) {
    pivot->right->parent = pivot->parent;
    pivot->parent = pivot->right;

    pivot->right = pivot->parent->left;
    pivot->parent->left = pivot;
    
    if(pivot->parent->parent == nullptr) {
        root_ = pivot->parent;
    } else {
        if(pivot->parent->parent->left == pivot) {
            pivot->parent->parent->left = pivot->parent;
        } else {
            assert(pivot->parent->parent->right == pivot);
            pivot->parent->parent->right = pivot->parent;
        }
    }
}

template<class T, class Cmp> 
typename rb_tree<T,Cmp>::iterator rb_tree<T,Cmp>::insert(T&& value) {
    if(root_ == nullptr) {
        root_ = new rb_node<T,Cmp>(value,rb_node<T,Cmp>::black);
        size_ = 1;
        return iterator(this, root_);
    }
    auto insertion = root_->unbalance_insert(value);
    if(insertion.first == nullptr) {
        return end(); //pretty sure this can't happen
    } else if(insertion.second == false) {
        return iterator(this, insertion.first);
    } else {
        balance_at(insertion.first);
        ++size_;
        return iterator(this, insertion.first);
    }
}

template<class T, class Cmp>
void rb_tree<T,Cmp>::balance_at(rb_node<T,Cmp>* loc) {
    constexpr auto black = rb_node<T,Cmp>::black;
    constexpr auto red = rb_node<T,Cmp>::red;
    if(loc.is_root()) {
        loc->color_ = black;
    } else if(loc->parent_->color_ == black) {
        loc->color_ = red;
    } else {//parent is red
        //loc implicitly has a grandparent
        rb_node<T,Cmp> uncle = loc->parent->brother();
        if(uncle == nullptr || uncle->color == black) {
            if(uncle == nullptr) {
                loc->color = red;
            } else {
                loc->color = black;
            }
            if(loc->parent->is_left()) {
                rotate_right(loc->parent->parent);
            }
            else {
                rotate_left(loc->parent->parent);
            }
        } else { //uncle is red
            loc->parent->color = black;
            uncle->color = black;
            balance_at(loc->parent->parent);         
        }
    }
}





