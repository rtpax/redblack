
#include "rb_tree.hh"
#include "rb_node.hh"
#include <cassert>

template<class T, class Cmp, class Alloc> 
inline void rb_tree<T,Cmp,Alloc>::rotate_right(rb_node<T,Cmp,Alloc>* pivot) {
    rb_node<T,Cmp,Alloc>* parent = pivot->parent;
    rb_node<T,Cmp,Alloc>* left = pivot->left;
    //assume left exists    
    rb_node<T,Cmp,Alloc>* left_right = pivot->left->right;

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

template<class T, class Cmp, class Alloc> 
inline void rb_tree<T,Cmp,Alloc>::rotate_left(rb_node<T,Cmp,Alloc>* pivot) {
    rb_node<T,Cmp,Alloc>* parent = pivot->parent;
    rb_node<T,Cmp,Alloc>* right = pivot->right;
    //assume right exists    
    rb_node<T,Cmp,Alloc>* right_left = pivot->right->left;

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

template<class T, class Cmp, class Alloc>
inline typename rb_tree<T,Cmp,Alloc>::iterator rb_tree<T,Cmp,Alloc>::insert(const T& value) {
    assert(root_ != nullptr);
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

template<class T, class Cmp, class Alloc>
inline typename rb_tree<T,Cmp,Alloc>::iterator rb_tree<T,Cmp,Alloc>::erase(iterator it) {
    assert(it.loc_ != end_);
    assert(it.loc_ != rend_);
    iterator ret = it;
    ++ret;
    --size_;
    auto remove_node = it.loc_;
    if(!(remove_node->right == nullptr) != !(remove_node->left == nullptr)) {
        auto color = remove_node->color;
        auto node = unbalanced_delete(remove_node);
        if(node != nullptr && color == black) {
            treat_as_delete(node);
        } else {
            assert(size_ == 0);
        }
    } else {
        if(remove_node->right != nullptr && remove_node->left != nullptr) {
            auto replace_node = remove_node->sequential_next();//could have done prev
            assert(replace_node != nullptr);
            assert(replace_node->left == nullptr);//would be right instead for prev
            swap_nodes(remove_node, replace_node);
        }
        auto color = remove_node->color;
        if(remove_node == nullptr) {
            assert(it.loc_ == root_);
            delete root_;
            root_ = nullptr;    
        } else if(remove_node->is_right()) {
            auto node = remove_node->parent;
            unbalanced_delete(remove_node);
            if(color == black)
                balance_right_deletion(node);
        } else {
            assert(remove_node->is_left());
            auto node = remove_node->parent;
            unbalanced_delete(remove_node);
            if(color == black)
                balance_left_deletion(node);
        }
    }
    return ret;
}

template<class T, class Cmp, class Alloc>
inline void rb_tree<T,Cmp,Alloc>::swap_nodes(rb_node<T,Cmp,Alloc>* a, rb_node<T,Cmp,Alloc>* b) {
    assert(a->is_root() || a->is_right() || a->is_left());
    assert(b->is_root() || b->is_right() || b->is_left());

    enum relation { right_child, left_child, right_parent, left_parent, none };
    relation a_to_b = 
            a->right == b ? right_parent : 
            a->left == b ? left_parent :
            b->right == a ? right_child :
            b->left == a ? left_child :
            none;

    auto a_parent = a->parent;
    auto a_right = a->right;
    auto a_left = a->left;
    auto a_color = a->color;

    auto b_parent = b->parent;
    auto b_right = b->right;
    auto b_left = b->left;
    auto b_color = b->color;

    switch (a_to_b) {
    case right_parent:
        a_right = a;
        b_parent = b;
        break;
    case left_parent:
        a_left = a;
        b_parent = b;
        break;
    case right_child:
        b_right = b;
        a_parent = a;
        break;
    case left_child:
        b_left = b;
        a_parent = a;
        break;
    case none:
        break;
    }

    if(!a->is_root()) {
        if(a->is_right())
            a->parent->right = b;
        else
            a->parent->left = b;
    } else {
        root_ = b;
    }

    if(!b->is_root()) {
        if(b->is_right())
            b->parent->right = a;
        else
            b->parent->left = a;
    } else { 
        root_ = a;
    }

    if(a->has_right())
        a->right->parent = b;
    if(a->has_left())
        a->left->parent = b;
 
    if(b->has_right())
        b->right->parent = a;
    if(b->has_left())
        b->left->parent = a;

    a->parent = b_parent;
    a->right = b_right;
    a->left = b_left;
    a->color = b_color;

    b->parent = a_parent;
    b->right = a_right;
    b->left = a_left;
    b->color = a_color;


}

template<class T, class Cmp, class Alloc>
inline rb_node<T,Cmp,Alloc>* rb_tree<T,Cmp,Alloc>::unbalanced_delete(rb_node<T,Cmp,Alloc>* arg) {
    rb_node<T,Cmp,Alloc>* to_replace_arg = nullptr;
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

template<class T, class Cmp, class Alloc>
inline void rb_tree<T,Cmp,Alloc>::balance_insertion(rb_node<T,Cmp,Alloc>* loc) {
    if(loc->is_root()) {
        loc->color = black;
    } else if(loc->parent->color == black) {
        loc->color = red;
    } else {//parent is red
        //loc implicitly has a black grandparent
        assert(loc->parent->color == red);
        assert(loc->parent->parent->color == black);
        rb_node<T,Cmp,Alloc>* uncle = loc->parent->brother();
        if(uncle == nullptr || uncle->color == black) {
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
        } else { //uncle is red
            loc->parent->color = black;
            uncle->color = black;
            loc->color = red;
            balance_insertion(loc->parent->parent);         
        }
    }
}

template<class T, class Cmp, class Alloc>
inline void rb_tree<T,Cmp,Alloc>::treat_as_delete(rb_node<T,Cmp,Alloc>* node) {
    if(node->is_root()) {
        node->color = black;
    } else if(node->is_right()) {
        balance_right_deletion(node->parent);
    } else {
        assert(node->is_left());
        balance_left_deletion(node->parent);
    }
}

template<class T, class Cmp, class Alloc>
inline void rb_tree<T,Cmp,Alloc>::balance_left_deletion(rb_node<T,Cmp,Alloc>* loc) {
    if(loc->left != nullptr && loc->left->color == red) {
        loc->left->color = black;
    } else {
        assert(loc->right != nullptr);
        if(loc->right->color == red) {
            assert(loc->color == black);
            loc->color = red;
            loc->right->color = black;
            rotate_left(loc);
            //fall through to next if
        }
        if (loc->color == red) {
            assert(loc->right->color == black);
            if(loc->right->right != nullptr && loc->right->right->color == red) {
                loc->right->right->color = black;
                loc->right->color = red;
                loc->color = black;
                rotate_left(loc);
            } else if(loc->right->left == nullptr || loc->right->left->color == black) {
                loc->right->color = black;
                loc->color = red;
                rotate_left(loc);
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
                loc->right->left->color = black;
                loc->right->color = red;
                rotate_right(loc->right);
                assert(loc->right->color == black);
                assert(loc->color == black);
                //we are now back at situation loc->right->right->color == red, do the same as there
                loc->right->right->color = black;
                rotate_left(loc);
            }
        }
    }
}

// Do not write this method by hand. Write balance_left_deletion, then copy and 
// paste and swap all occurances (even with words) right with left
template<class T, class Cmp, class Alloc>
inline void rb_tree<T,Cmp,Alloc>::balance_right_deletion(rb_node<T,Cmp,Alloc>* loc) {
    if(loc->right != nullptr && loc->right->color == red) {
        loc->right->color = black;
    } else {
        assert(loc->left != nullptr);
        if(loc->left->color == red) {
            assert(loc->color == black);
            loc->color = red;
            loc->left->color = black;
            rotate_right(loc);
            //fall through to next if
        }
        if (loc->color == red) {
            assert(loc->left->color == black);
            if(loc->left->left != nullptr && loc->left->left->color == red) {
                loc->left->left->color = black;
                loc->left->color = red;
                loc->color = black;
                rotate_right(loc);
            } else if(loc->left->right == nullptr || loc->left->right->color == black) {
                loc->left->color = black;
                loc->color = red;
                rotate_right(loc);
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
                loc->left->right->color = black;
                loc->left->color = red;
                rotate_left(loc->left);
                assert(loc->left->left->color == red);
                assert(loc->left->color == black);
                assert(loc->color == black);
                //we are now back at situation loc->left->left->color == red, do the same as there
                loc->left->left->color = black;
                rotate_right(loc);
            }
        }
    }
}

template<class T, class Cmp, class Alloc>
inline typename rb_tree<T,Cmp,Alloc>::iterator rb_tree<T,Cmp,Alloc>::find(const T& value) {
    rb_node<T,Cmp,Alloc>* cur = root_;
    Cmp cmp;
    while(cur != nullptr) {
        if(cur->is_end() || (cur->has_elem() && cmp(value, *cur->elem))) {
            cur = cur->left;
        } else if(cur->is_rend() || (cur->has_elem() && cmp(*cur->elem, value))) {
            cur = cur->right;
        } else {
            break;
        }
    }
    if(cur == nullptr)
        return end();
    return iterator(cur);
}

template<class T, class Cmp, class Alloc>
inline typename rb_tree<T,Cmp,Alloc>::const_iterator rb_tree<T,Cmp,Alloc>::find(const T& value) const {
    return const_iterator(const_cast<rb_tree*>(this)->find(value));
}

template<class T, class Cmp, class Alloc>
inline bool rb_tree<T,Cmp,Alloc>::contains(const T& value) const {
    return find(value) == cend();
}
