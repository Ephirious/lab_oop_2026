#ifndef LAB1_SORT_H
#define LAB1_SORT_H

#include <cstddef>

namespace Array {
    using std::size_t;
    
    int* create(size_t size);
    void clear(int*& arr);
    int* resize(int* arr, size_t size, size_t new_size);
    int* insert(int* arr, size_t& size, size_t pos, int value);
    int* remove(int* arr, size_t& size, size_t pos);
    void print(const int* arr, size_t size);
    bool contains(const int* arr, size_t size, int target, size_t& out_index);
    void fill(int* arr, size_t size, int lowerBound, int upperBound);
    void sort(int* arr, size_t size);
    void copy(const int* src, int* dest, size_t srcSize, size_t srcPos, size_t destSize, size_t destPos, size_t count);
    size_t removeDuplicates(int* arr, size_t size);
}



#endif //LAB1_SORT_H