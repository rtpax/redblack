
#include "rb_tree.hh"
#include "rb_node.hh"
#include <cassert>

template<class T, class Cmp> 
inline void rb_tree<T,Cmp>::rotate_right(rb_node<T,Cmp>* pivot) {
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
inline void rb_tree<T,Cmp>::rotate_left(rb_node<T,Cmp>* pivot) {
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
inline typename rb_tree<T,Cmp>::iterator rb_tree<T,Cmp>::insert(const T& value) {
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
        balance_insertion(insertion.first);
        ++size_;
        return iterator(insertion.first);
    }
}

template<class T, class Cmp>
inline typename rb_tree<T,Cmp>::iterator rb_tree<T,Cmp>::erase(iterator it) {
    iterator ret = it;
    ++ret;
    --size_;
    if(it.loc_->right == nullptr || it.loc_->left == nullptr) {
        auto node = unbalanced_delete(it.loc_);
        if(node != nullptr)
            treat_as_delete(node);
    } else {
        ++it;//could just as well do --it
        assert(it.loc_->left == nullptr);//would be right instead for --it;
        if(it.loc_->is_right()) {
            auto node = it.loc_->parent;
            unbalanced_delete(it.loc_);
            balance_right_deletion(node);
        } else {
            assert(it.loc_->is_left());
            auto node = it.loc_->parent;
            unbalanced_delete(it.loc_);
            balance_left_deletion(node);
        }
    }
    return ret;
}

template<class T, class Cmp>
inline rb_node<T,Cmp>* rb_tree<T,Cmp>::unbalanced_delete(rb_node<T,Cmp>* arg) {
    rb_node<T,Cmp>* to_replace_arg = nullptr;
    if(arg->left != nullptr) {
        assert(arg->right == nullptr);
        to_replace_arg = arg->left;
    } else if (arg->right != nullptr) {
        assert(arg->left == nullptr);
        to_replace_arg = arg->right;
    } else {
        assert(arg->left == nullptr);
        assert(arg->right == nullptr);
        to_replace_arg = nullptr;
    }
    if(arg->is_root()) {
        root_ = to_replace_arg;
        if(to_replace_arg != nullptr)
            to_replace_arg->parent = nullptr;
    } else if(arg->is_right()) {
        arg->parent->right = to_replace_arg;
        if(to_replace_arg != nullptr)
            to_replace_arg->parent = arg->parent;
    } else {
        assert(arg->is_left());
        arg->parent->left = to_replace_arg;
        if(to_replace_arg != nullptr)
            to_replace_arg->parent = arg->parent;
    }
    delete arg;
    return to_replace_arg;
}

template<class T, class Cmp>
inline void rb_tree<T,Cmp>::balance_insertion(rb_node<T,Cmp>* loc) {
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
            uncle->color = red;
            loc->color = red;
            if(loc->parent->is_left()) {
                rotate_right(loc->parent->parent);
            } else {
                rotate_left(loc->parent->parent);
            }
            balance_insertion(loc->parent->parent);
        } else { //uncle is red
            loc->parent->color = black;
            uncle->color = black;
            loc->color = red;
            balance_insertion(loc->parent->parent);         
        }
    }
}

template<class T, class Cmp>
inline void rb_tree<T,Cmp>::treat_as_delete(rb_node<T,Cmp>* node) {
    if(node->is_root()) {
        node->color = black;
    } else if(node->is_right()) {
        balance_right_deletion(node->parent);
    } else {
        assert(node->is_left());
        balance_left_deletion(node->parent);
    }
}

template<class T, class Cmp>
inline void rb_tree<T,Cmp>::balance_left_deletion(rb_node<T,Cmp>* loc) {
    if(loc->left != nullptr && loc->left->color == red) {
        loc->left->color = black;
    } else {
        assert(loc->right != nullptr);
        if(loc->right->color == red) {
            assert(loc->color == black);
            loc->color = red;
            loc->right->color = black;
            rotate_left(loc);
        } else if (loc->color == red) {
            assert(loc->right->color = black);
            if(loc->right->right != nullptr && loc->right->right->color == red) {
                loc->right->right->color = black;
                loc->right->color = red;
                loc->color = black;
                rotate_left(loc);
            } else if(loc->right->left == nullptr || loc->right->left->color == black) {
                loc->right->color = black;
                loc->color = red;
                rotate_left(loc);
                treat_as_delete(loc->parent);
            } else {
                loc->right->left->color = black;
                loc->right->color = red;
                rotate_right(loc->right);
                //we are now back at situation loc->right->color == black
                //  loc->right->right->color == red, do the same as there
                loc->right->right->color = black;
                loc->right->color = red;
                loc->color = black;
                rotate_left(loc);
            }
        } else {
            assert(loc->color == black);
            assert(loc->right->color == black);
            if(loc->right->right != nullptr && loc->right->right->color == red) {
                loc->right->right->color = black;
                rotate_left(loc);
            } else if(loc->right->left == nullptr || loc->right->left->color == black) {
                loc->color = red;
                rotate_left(loc);
                treat_as_delete(loc->parent);
            } else {
                assert(loc->right->left->color == red);
                assert(loc->right->color == black);
                rotate_right(loc->right);
                assert(loc->right->color == red);
                assert(loc->color == black);
                //we are now back at situation loc->right->color == red, do the same as there
                loc->color = red;
                loc->right->color = black; //different loc->right from before
                rotate_left(loc);
            }
        }
    }
}

// Do not write this method by hand. Write balance_left_deletion, then copy and 
// paste and swap all occurances (even with words) right with left
template<class T, class Cmp>
inline void rb_tree<T,Cmp>::balance_right_deletion(rb_node<T,Cmp>* loc) {
    if(loc->right != nullptr && loc->right->color == red) {
        loc->right->color = black;
    } else {
        assert(loc->left != nullptr);
        if(loc->left->color == red) {
            assert(loc->color == black);
            loc->color = red;
            loc->left->color = black;
            rotate_right(loc);
        } else if (loc->color == red) {
            assert(loc->left->color = black);
            if(loc->left->left != nullptr && loc->left->left->color == red) {
                loc->left->left->color = black;
                loc->left->color = red;
                loc->color = black;
                rotate_right(loc);
            } else if(loc->left->right == nullptr || loc->left->right->color == black) {
                loc->left->color = black;
                loc->color = red;
                rotate_right(loc);
                treat_as_delete(loc->parent);
            } else {
                loc->left->right->color = black;
                loc->left->color = red;
                rotate_left(loc->left);
                //we are now back at situation loc->left->color == black
                //  loc->left->left->color == red, do the same as there
                loc->left->left->color = black;
                loc->left->color = red;
                loc->color = black;
                rotate_right(loc);
            }
        } else {
            assert(loc->color == black);
            assert(loc->left->color == black);
            if(loc->left->left != nullptr && loc->left->left->color == red) {
                loc->left->left->color = black;
                rotate_right(loc);
            } else if(loc->left->right == nullptr || loc->left->right->color == black) {
                loc->color = red;
                rotate_right(loc);
                treat_as_delete(loc->parent);
            } else {
                assert(loc->left->right->color == red);
                assert(loc->left->color == black);
                rotate_left(loc->left);
                assert(loc->left->color == red);
                assert(loc->color == black);
                //we are now back at situation loc->left->color == red, do the same as there
                loc->color = red;
                loc->left->color = black; //different loc->left from before
                rotate_right(loc);
            }
        }
    }
}

template<class T, class Cmp>
inline rb_node<T,Cmp>* rb_tree<T,Cmp>::first_node() {
    return const_cast<rb_node<T,Cmp>*>(const_cast<const rb_tree*>(this)->first_node());
}

template<class T, class Cmp>
inline rb_node<T,Cmp>* rb_tree<T,Cmp>::last_node() {
    return const_cast<rb_node<T,Cmp>*>(const_cast<const rb_tree*>(this)->last_node());
}

template<class T, class Cmp>
inline const rb_node<T,Cmp>* rb_tree<T,Cmp>::first_node() const {
    if(root_ == nullptr)
        return nullptr;
    auto ret = root_;
    while(ret->left != nullptr) {
        ret = ret->left;
    }
    return ret;
}

template<class T, class Cmp>
inline const rb_node<T,Cmp>* rb_tree<T,Cmp>::last_node() const {
    if(root_ == nullptr)
        return nullptr;
    auto ret = root_;
    while(ret->left != nullptr) {
        ret = ret->left;
    }
    return ret;
}




