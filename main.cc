#include "rb_tree.hh"
#include <iostream>
#include <string>

int main(int argc, char ** argv) {
    rb_tree<int> a;

    char break_char = '/';
    int i = 1;
    for(; i < argc && argv[i][0] != break_char; ++i) {
        a.insert(std::stoi(argv[i]));
    }

    for(int i : a) {
        std::cout << i << ", ";
    }
    std::cout << "\n";

    for(i = i + 1; i < argc; ++i) {
        a.root_->dump_tree(0);
        std::cout << "+++++++++++++++++++++++++++++\n";
        auto it = a.find(std::stoi(argv[i]));
        if(it != a.end())
            a.erase(it);
    }
    a.root_->dump_tree(0);
    std::cout << "+++++++++++++++++++++++++++++\n";

    for(int i : a) {
        std::cout << i << ", ";
    }
    std::cout << "\n";
}