#include "rb_tree.hh"
#include <vector>

/**
 * This class exists solely to be a friend of rb_* and store test functions with private access
 **/
template<class T, class Cmp = std::less<T>, class Alloc = std::allocator<T>>
class rb_test {
    typedef rb_tree<T,Cmp,Alloc> tree_type;
    typedef rb_node<T,Cmp,Alloc> node_type;
    typedef typename tree_type::iterator iterator;
    typedef typename tree_type::reverse_iterator reverse_iterator;
    typedef typename tree_type::const_iterator const_iterator;
    typedef typename tree_type::const_reverse_iterator const_reverse_iterator;
public:
    static tree_type example_tree;

    rb_test() = delete;

    //negative value indicates error
    static int black_depth(const tree_type& tree) { return black_depth(tree.root_); }
    static int black_depth(const node_type* root) {
        if(root == nullptr)
            return 0;

        int l_depth = black_depth(root->left);
        if(l_depth == -1)
            return -1;
        
        int r_depth = black_depth(root->right);
        if(r_depth == -1)
            return -1;
        
        if(r_depth != l_depth)
            return -1;

        return r_depth + (root->is_black() ? 1 : 0);
    }
    static bool has_red_adjacent(const tree_type& tree) { return has_red_adjacent(tree.root_, true); }
    static bool has_red_adjacent(const node_type* root, bool last_was_red) {
        if(root == nullptr)
            return false;
        if(root->is_red() && last_was_red)
            return true;
        return has_red_adjacent(root->left, root->is_red()) 
                || has_red_adjacent(root->right, root->is_red());
    }

    static int count_nodes(const node_type* root) {
        if(root == nullptr)
            return 0;
        return count_nodes(root->left) + count_nodes(root->right);
    }

    //check that each left node is less, each right node is greater
    static bool is_partially_ordered(const tree_type& tree) { return is_partially_ordered(tree, tree.root_); }
    static bool is_partially_ordered(const tree_type& tree, const node_type* root) {
        if(root->has_left() && !root->left->is_rend() && 
                !root->is_end() && !tree.cmp_(*root->left->elem, *root->elem)) {
            return false;
        }
        if(root->has_right() && !root->right->is_end() && 
                !root->is_rend() && !tree.cmp_(*root->elem, *root->right->elem)) {
            return false;
        }
        return (root->has_left() ? is_partially_ordered(tree, root->left) : true) &&
                (root->has_right() ? is_partially_ordered(tree, root->right) : true);        
    }
    static bool is_fully_ordered(const tree_type& tree) {
        auto prev_it = tree.cbegin();
        if(prev_it == tree.cend())
            return true;
        for(auto it = std::next(prev_it); it != tree.cend(); prev_it = it++) {
            if(!tree.cmp_(*prev_it,*it))
                return false;
        }
        return true;
    }
};

template<class T>
static rb_tree<T> example_tree = { 43,64,2,16,32,2,65,17,19,23,54,21,16,21,14 };
template<class T> 
static std::vector<T> example_tree_result = { 2,14,17,19,21,23,32,43,54,64,65 };

