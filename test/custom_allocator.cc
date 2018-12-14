#include "awful_allocator.hh"

template <class T> std::list<char*> custom_allocator<T>::memory;