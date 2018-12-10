#include "rb_node.hh"

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
        return std::pair<rb_node<T,Cmp>*, bool>std::pair<rb_node<T,Cmp>*, bool>(this, false);
    }
}
