#include <utility>

class rb_test;

template<class T, class Cmp>
struct rb_node {
    friend rb_test;
    enum color { red = 0, black = 1 };

    color color_;
    T elem_;
    rb_node *left_, *right_, *parent_;

    bool is_root() { return parent_ == nullptr; }
    bool is_leaf() { return left_ == nullptr || right_ == nullptr; }
    std::pair<rb_node*,bool> unbalanced_insert(T&& value);
}

template<class T, class Cmp, bool is_const, bool is_reverse>
class rb_iterator {
    friend rb_test;
private:
    rb_tree* tree_;
    rb_node* loc_;
public:
    T& operator*();
    const T& operator*();

    rb_iterator operator++();
    rb_iterator operator--();
    rb_iterator operator++(int);
    rb_iterator operator--(int);
}

template<class T, class Cmp = std::less<T>>
class rb_tree {
    friend rb_test;
private:
    rb_node<T>* root_;
    size_t size_;

public:
    enum direction { left = 0, right = 1 };
    typedef rb_iterator<T,Cmp,false,false> iterator;
    typedef rb_iterator<T,Cmp,true ,false> const_iterator;
    typedef rb_iterator<T,Cmp,false,true > reverse_iterator;
    typedef rb_iterator<T,Cmp,true ,true > const_reverse_iterator;

    void rotate(rb_node<T>*, rb_node::direction);
    void insert(const T& value) { return insert(std::move(T(value))); }
    void insert(T&& value);
    void remove(const T& value);

    iterator begin();
    iterator end();
    const_iterator cbegin() const;
    const_iterator cend() const;
    reverse_iterator rbegin();
    reverse_iterator rend();
    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;            
}
