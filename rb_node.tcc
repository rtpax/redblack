#include "rb_node.hh"
#include <stdexcept>
#include <cassert>

template<class T, class Cmp>
std::pair<rb_node<T,Cmp>*, bool>  rb_node<T,Cmp>::unbalanced_insert(const T& value) {
    Cmp cmp;
    //if elem == nullptr, node is end or rend
    if(is_end() || (has_elem() && cmp(value, *elem))) {
        if(left == nullptr) {
            left = new rb_node<T,Cmp>(value, red, this);
            return std::pair<rb_node<T,Cmp>*, bool>(left, true);
        } else {
            return left->unbalanced_insert(value);
        }
    } else if(is_rend() || (has_elem() && cmp(*elem, value))) {
        if(right == nullptr) {
            right = new rb_node<T, Cmp>(value, red, this);
            return std::pair<rb_node<T,Cmp>*, bool>(right, true);
        } else {
            return right->unbalanced_insert(value);
        }
    } else {
        return std::pair<rb_node<T,Cmp>*, bool>(this, false);
    }
}

template<class T, class Cmp>
rb_node<T,Cmp>* rb_node<T,Cmp>::sequential_next() {
    if(right != nullptr) {
        rb_node* cur = right;
        while(cur->left != nullptr) {
            cur = cur->left;
            assert(cur != right);
        }
        return cur;
    } else {
        rb_node* cur = this;
        while(!cur->is_root()) {
            if(cur->is_left()) {
                return cur->parent;
            }
            else {
                cur = cur->parent;
            }
            assert(cur != this);
        }
        return nullptr;
    }
}

template<class T, class Cmp>
rb_node<T,Cmp>* rb_node<T,Cmp>::sequential_prev() {
    if(left != nullptr) {
        rb_node* cur = left;
        while(cur->right != nullptr) {
            cur = cur->right;
            assert(cur != left);
        }
        return cur;
    } else {
        rb_node* cur = this;
        while(!cur->is_root()) {
            if(cur->is_right()) {
                return cur->parent;
            }
            else {
                cur = cur->parent;
            }
            assert(cur != this);
        }
        return nullptr;
    }
}

template <class T, class Cmp>
void rb_node<T,Cmp>::dump_tree(int depth) {
    if(is_root()) {
        std::cout << "--";
    } else if(is_left()) {
        std::cout << "<-";
    } else if (is_right()) {
        std::cout << "->";
    } else {
        std::cout << "##";
    }

    for(int i = 0; i < depth; ++i) {
        std::cout << " ";
    }

    std::cout << (elem ? std::to_string(*elem) : (is_end() ? std::string("end ") : std::string("rend "))) << (color == black ? "b" : "r") << "\n";
    if(left)
        left->dump_tree(depth+1);
    if(right)
        right->dump_tree(depth+1);
}

template <class T, class Cmp>
void rb_node<T,Cmp>::dump_node() {
    std::cout << this << ":\n";
    std::cout << "node-type: " << (is_end() ? "end" : is_rend() ? "rend" : "elem") << "\n";
    std::cout << "child-type: " << (is_root() ? "root" : is_left() ? "left" : is_right() ? "right" : "error") << "\n";
    std::cout << elem << "\n";
    std::cout << "color: " << (color == red ? std::string("red") 
            : color == black ? std::string("black") 
            : std::string("error-color:") + std::to_string((int)color)) << "\n";
    std::cout << "parent: " << parent << "\n";
    std::cout << "right: " << right << "\n";
    std::cout << "left: " << left << "\n";
}