#ifndef AWFUL_ALLOCATOR_HH
#define AWFUL_ALLOCATOR_HH

#include <list>
#include <stdexcept>

template <class T>
struct custom_allocator {
    static std::list<char*> memory;
    T* allocate(size_t n) {
        char* mem = new char[sizeof(T) * n];
        memory.push_back(mem);
        return (T*) mem;
    }
    void deallocate(T* mem, size_t n) {
        //ignore n
        deallocate(mem);
    }
    void deallocate(T* mem) {
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

#endif