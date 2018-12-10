#include "rb_node.hh"

template<class T, class Cmp> 
std::pair<rb_node<T,Cmp>*, bool>  rb_node<T,Cmp>::unbalanced_insert(T&& value) {
    Cmp cmp = Cmp();
    if(cmp(value, elem)) {
        if(left == nullptr) {
            left = new rb_node<T,Cmp>(value, red);
            return std::pair<rb_node<T,Cmp>*, bool>(left, true);
        } else {
            return left->unbalanced_insert(value, cmp);
        }
    } else if (cmp(elem, value)) {
        if(right == nullptr) {
            right = new rb_node<T, Cmp>(value, red);
            return std::pair<rb_node<T,Cmp>*, bool>(right, true);
        } else {
            return right->unbalanced_insert(value, cmp);
        }
    } else {
        return std::pair<rb_node<T,Cmp>*, bool>(this, false);
    }
}
