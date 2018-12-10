#include "rb_tree.hh"
#include <iostream>
#include <string>

int main(int argc, char ** argv) {
    rb_tree<int> a;

    for(int i = 1; i < argc; ++i) {
        a.insert(std::stoi(argv[i]));
    }

    for(int i : a) {
        std::cout << i << ", ";
    }

    std::cout << "\n";
}