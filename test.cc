#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "rb_tree.hh"

TEST_CASE("insertion", "rb_tree:: insert,rb_tree,size; rb_iterator:: *") {
    rb_tree<int> test;
    
    auto \
    result = test.insert(3);
    CHECK(*result.first == 3);
    CHECK(*result.second)
    CHECK(test.size() == 1);
    
    result = test.insert(1);
    CHECK(*result.first == 1);
    CHECK(*result.second)
    CHECK(test.size() == 2);

    result = test.insert(2);
    CHECK(*result.first == 2);
    CHECK(*result.second)
    CHECK(test.size() == 3);
    
    result = test.insert(3);    
    CHECK(*result.first == 3);
    CHECK(!*result.second)
    CHECK(test.size() == 3);
}

TEST_CASE("redblack property", "rb_tree:: insert,rb_tree; rb_node::; insertion") {
    rb_tree<int> test;
    test.insert(3);
    test.insert(1);
    test.insert(2);
    test.insert(5);
    test.insert(4);

    CHECK(test.isredblack());
}

TEST_CASE("iterate", "rb_iterator:: ++,--,*,->") {
    rb_tree<int> test;
    test.insert(1);
    test.insert(2);
    test.insert(3);
    test.insert(4);
}



















