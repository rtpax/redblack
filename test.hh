#include "rb_tree.hh"

/**
 * This class exists solely to be a friend of rb_* and store test functions with private access
 **/
template<class T, class Cmp, class Alloc>
class rb_test {
public:
    
    /**
     * Checks that the tree satisfies all the conditions of begin red_black, namely
     *   - Every node is either red or black
     *   - If a node is red, it's children are black (or null)
     *   - Every simple path from a node to descendant leaf has the same number of black nodes
     **/
    tree_is_red_black(const rb_tree& test) {

    }
};

