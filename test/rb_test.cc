#include "rb_test.hh"
#include <catch.hpp>

TEST_CASE("insertion", "rb_tree:: insert,rb_tree,size; rb_iterator:: *") {
    rb_tree<int> test;
    
    auto \
    result = test.insert(3);
    CHECK(*result.first == 3);
    CHECK(result.second);
    CHECK(test.size() == 1);
    
    result = test.insert(1);
    CHECK(*result.first == 1);
    CHECK(result.second);
    CHECK(test.size() == 2);

    result = test.insert(2);
    CHECK(*result.first == 2);
    CHECK(result.second);
    CHECK(test.size() == 3);
    
    result = test.insert(3);    
    CHECK(*result.first == 3);
    CHECK(!result.second);
    CHECK(test.size() == 3);
}

TEST_CASE("redblack property", "checks against example") {
    CHECK(rb_test<int>::black_depth(example_tree<int>) != -1);
    CHECK(!rb_test<int>::has_red_adjacent(example_tree<int>));    
}

TEST_CASE("ordered", "checks against example") {
    CHECK(rb_test<int>::is_partially_ordered(example_tree<int>));
    CHECK(rb_test<int>::is_fully_ordered(example_tree<int>));
}

TEST_CASE("custom allocator", "no assertions, this will throw exceptions") {
    
}


