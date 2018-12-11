#include "rb_node.hh"
#include <stdexcept>

template<class T, class Cmp> 
std::pair<rb_node<T,Cmp>*, bool>  rb_node<T,Cmp>::unbalanced_insert(const T& value) {
    Cmp cmp = Cmp();
    if(cmp(value, elem)) {
        if(left == nullptr) {
            left = new rb_node<T,Cmp>(value, red, this);
            return std::pair<rb_node<T,Cmp>*, bool>(left, true);
        } else {
            return left->unbalanced_insert(value);
        }
    } else if (cmp(elem, value)) {
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
#ifdef DEBUG
            if(cur == right)
                throw std::logic_error("infinite nodes");
#endif
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
#ifdef DEBUG
            if(cur == this)
                throw std::logic_error("infinite nodes");
#endif
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
#ifdef DEBUG
            if(cur == left)
                throw std::logic_error("infinite nodes");
#endif
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
#ifdef DEBUG
            if(cur == this)
                throw std::logic_error("infinite nodes");
#endif
        }
        return nullptr;
    }
}

template <class T, class Cmp>
void rb_node<T,Cmp>::dump_tree(int depth) {
    if(!is_root() && is_left()) {
        std::cout << "<-";
    } else if (!is_root() && is_right()) {
        std::cout << "->";
    } else {
        std::cout << "--";
    }
    for(int i = 0; i < depth; ++i) {
        std::cout << " ";
    }
    std::cout << elem << (color == black ? "b" : "r") << "\n";
    if(left)
        left->dump_tree(depth+1);
    if(right)
        right->dump_tree(depth+1);
}
