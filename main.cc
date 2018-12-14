#include "rb_tree.hh"
#include <iostream>
#include <string>

#include <vector>
#include <random>
#include <chrono>
#include <set>

int main(int argc, char ** argv) {
    rb_tree<int> a = { 9,8,7,6,5,4,3,2,1,10 };

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
        auto it = a.find(std::stoi(argv[i]));
        if(it != a.end())
            a.erase(it);
    }

    for(int i : a) {
        std::cout << i << ", ";
    }
    std::cout << "\n\n";



    //compare insertion speeds

    auto now = [](){ return std::chrono::high_resolution_clock::now().time_since_epoch().count(); };

    std::vector<int> base;
    std::mt19937 rng(now());
    for(int i = 0; i < 100000; ++i) {
        base.push_back(rng());
    }


    auto rb_start = now();
    rb_tree<int> rb_tmp(base.begin(), base.end());
    auto rb_end = now();

    std::cout << "rb_tree<int>:\n"
            << "  time: " << (rb_end - rb_start) << "\n"
            << "  size: " << rb_tmp.size() << "\n";


    auto set_start = now();
    std::set<int> set_tmp(base.begin(), base.end());
    auto set_end = now();

    std::cout << "std::set<int>:\n"
            << "  time: " << (set_end - set_start) << "\n"
            << "  size: " << set_tmp.size() << "\n";

    
}