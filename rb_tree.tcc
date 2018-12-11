
#include "rb_tree.hh"
#include "rb_node.hh"
#include <cassert>

template<class T, class Cmp> 
void rb_tree<T,Cmp>::rotate_right(rb_node<T,Cmp>* pivot) {
    rb_node<T,Cmp>* parent = pivot->parent;
    rb_node<T,Cmp>* left = pivot->left;
    //assume left exists    
    rb_node<T,Cmp>* left_right = pivot->left->right;

    left->parent = parent;
    left->right = pivot;

    pivot->left = left_right;
    pivot->parent = left;

    if(left_right != nullptr)
        left_right->parent = pivot;

    if(parent == nullptr) {
        root_ = left;
    } else if (parent->left == pivot) {
        parent->left = left;
    } else if (parent->right == pivot) {
        parent->right = left;
    }
}

template<class T, class Cmp> 
void rb_tree<T,Cmp>::rotate_left(rb_node<T,Cmp>* pivot) {
    rb_node<T,Cmp>* parent = pivot->parent;
    rb_node<T,Cmp>* right = pivot->right;
    //assume right exists    
    rb_node<T,Cmp>* right_left = pivot->right->left;

    right->parent = parent;
    right->left = pivot;

    pivot->right = right_left;
    pivot->parent = right;

    if(right_left != nullptr)
        right_left->parent = pivot;

    if(parent == nullptr) {
        root_ = right;
    } else if (parent->left == pivot) {
        parent->left = right;
    } else if (parent->right == pivot) {
        parent->right = right;
    }
}

template<class T, class Cmp> 
typename rb_tree<T,Cmp>::iterator rb_tree<T,Cmp>::insert(const T& value) {
    if(root_ == nullptr) {
        root_ = new rb_node<T,Cmp>(value,rb_node<T,Cmp>::black,nullptr);
        size_ = 1;
        return iterator(root_);
    }
    auto insertion = root_->unbalanced_insert(value);
    if(insertion.first == nullptr) {
        return end(); //pretty sure this can't happen
    } else if(insertion.second == false) {
        return iterator(insertion.first);
    } else {
        balance_at(insertion.first);
        ++size_;
        return iterator(insertion.first);
    }
}

template<class T, class Cmp>
void rb_tree<T,Cmp>::balance_at(rb_node<T,Cmp>* loc) {
    if(loc->is_root()) {
        loc->color = black;
    } else if(loc->parent->color == black) {
        loc->color = red;
    } else {//parent is red
        //loc implicitly has a black grandparent
        assert(loc->parent->color == red);
        assert(loc->parent->parent->color == black);
        rb_node<T,Cmp>* uncle = loc->parent->brother();
        if(uncle == nullptr) {
#ifdef DEBUG
            if(loc->brother() != nullptr) {
                std::cout << "uncle == nullptr but brother != nullptr\n";
                root_->dump_tree();
                exit(1);
            }
#endif
            loc->parent->parent->color = red;
            if(loc->parent->is_right() && loc->is_left()) {
                loc->parent->color = red;
                loc->color = black;
                rotate_right(loc->parent);
                rotate_left(loc->parent);//now grandparent
            } else if(loc->parent->is_left() && loc->is_right()) {
                loc->parent->color = red;
                loc->color = black;
                rotate_left(loc->parent);
                rotate_right(loc->parent);
            } else if(loc->parent->is_left()) {//loc->is_left()
                loc->parent->color = black;
                loc->color = red;
                rotate_right(loc->parent->parent);
            } else {//loc->parent->is_right() && loc->isright()
                loc->parent->color = black;
                loc->color = red;
                rotate_left(loc->parent->parent);
            }
        } else if(uncle->color == black) {
            loc->parent->parent->color = red;
            loc->parent->color = black;
            loc->color = black;
            if(loc->parent->is_left()) {
                rotate_right(loc->parent->parent);
            } else {
                rotate_left(loc->parent->parent);
            }
        } else { //uncle is red
            loc->parent->color = black;
            uncle->color = black;
            loc->color = red;
            balance_at(loc->parent->parent);         
        }
    }
}

template<class T, class Cmp>
rb_node<T,Cmp>* rb_tree<T,Cmp>::first_node() {
    return const_cast<rb_node<T,Cmp>*>(const_cast<const rb_tree*>(this)->first_node());
}

template<class T, class Cmp>
rb_node<T,Cmp>* rb_tree<T,Cmp>::last_node() {
    return const_cast<rb_node<T,Cmp>*>(const_cast<const rb_tree*>(this)->last_node());
}

template<class T, class Cmp>
const rb_node<T,Cmp>* rb_tree<T,Cmp>::first_node() const {
    if(root_ == nullptr)
        return nullptr;
    auto ret = root_;
    while(ret->left != nullptr) {
        ret = ret->left;
    }
    return ret;
}

template<class T, class Cmp>
const rb_node<T,Cmp>* rb_tree<T,Cmp>::last_node() const {
    if(root_ == nullptr)
        return nullptr;
    auto ret = root_;
    while(ret->left != nullptr) {
        ret = ret->left;
    }
    return ret;
}




