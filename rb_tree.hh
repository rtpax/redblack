
template<class T>
struct rb_node {
    enum direction { left = 0, right = 1 };
    enum color { red = 0, black = 1 };

    color color_;
    T elem_;
    rb_node *left_, *right_, *parent_;

    bool is_root() { return parent_ == nullptr; }
    bool is_leaf() { return left_ == nullptr || right_ == nullptr; }
}

template<class T, class Cmp = std::less<T>>
class rb_iterator {
private:
    rb_tree* tree_;
    rb_node* loc_
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
private:
    rb_node<T>* root_;
    size_t size_;

    rotate(rb_node<T>*, rb_node::direction);
    unbalanced_insert(T&& value);
    insert(const T& value) { return insert(std::move(T(value))); }
    insert(T&& value);
    remove(const T& value);
}






// left_rotate( Tree T, node x ) {
//     node y;
//     y = x->right;
//     /* Turn y's left sub-tree into x's right sub-tree */
//     x->right = y->left;
//     if ( y->left != NULL )
//         y->left->parent = x;
//     /* y's new parent was x's parent */
//     y->parent = x->parent;
//     /* Set the parent to point to y instead of x */
//     /* First see whether we're at the root */
//     if ( x->parent == NULL ) T->root = y;
//     else
//         if ( x == (x->parent)->left )
//             /* x was on the left of its parent */
//             x->parent->left = y;
//         else
//             /* x must have been on the right */
//             x->parent->right = y;
//     /* Finally, put x on y's left */
//     y->left = x;
//     x->parent = y;
//     }

/*

