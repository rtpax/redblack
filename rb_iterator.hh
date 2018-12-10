#ifndef RB_ITERATOR_HH
#define RB_ITERATOR_HH

template <class T, class Cmp>
class rb_test;
template <class T, class Cmp>
class rb_node;
template <class T, class Cmp>
class rb_tree;

template<class T, class Cmp, bool is_const, bool is_reverse>
class rb_iterator {
    friend rb_test<T,Cmp>;
private:
    typedef rb_tree<T,Cmp> tree_type;
    typedef rb_node<t,Cmp> node_type;
    tree_type* tree_;
    node_type* loc_;
public:
    rb_iterator(rb_tree* tree, rb_node* loc)

    T& operator*();
    const T& operator*();

    rb_iterator operator++();
    rb_iterator operator--();
    rb_iterator operator++(int);
    rb_iterator operator--(int);
};

#endif