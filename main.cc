#include "rb_tree.hh"
#include <iostream>
#include <string>

#include <list>
#include <array>

template <class T>
struct awful_allocator {
    static std::list<char*> memory;
    T* allocate(size_t n) {
        char* mem = new char[sizeof(T) * n];
        memory.push_back(mem);
        return (T*) mem;
    }
    void deallocate(const T* mem, size_t n) {
        //ignore n
        deallocate(mem);
    }
    void deallocate(const T* mem) {
        for(auto it = memory.begin(); it != memory.end(); ++it) {
            if(*it == (char*)mem) {
                delete[] *it;
                memory.erase(it);
                return;
            }
        }
        throw std::runtime_error("could not deallocate the requested memory");
    }
};

template <class T> std::list<char*> awful_allocator<T>::memory;

int main(int argc, char ** argv) {
    rb_tree<int, std::less<int>, awful_allocator<int>> a = { 9,8,7,6,5,4,3,2,1,10 };

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
    std::cout << "\n";

    for(char* i : awful_allocator<int>::memory) {
        std::cout << *((int*)i) << ", ";
    }
    std::cout << "\n";


}