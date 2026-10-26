#include <iostream>
#include <random>
#include <algorithm>

#include "../include/array.h"

namespace Array {
    int* create(size_t size) {
        if (size == 0) {
            return nullptr;
        }
        return new int[size];
    }

    void clear(int*& arr) {
        if (arr != nullptr) {
            delete[] arr;
            arr = nullptr;
        }
    }


    int* resize(int* arr, size_t size, size_t new_size) {
        if (new_size == 0) {
            clear(arr);
            return nullptr;
        }

        if (arr == nullptr) {
            return new int[new_size];
        }

        if (size == new_size) {
            return arr;
        }


        int* newArray = new int[new_size];
        const size_t bound = size < new_size ? size : new_size;

        for (size_t i = 0; i < bound; ++i) {
            newArray[i] = arr[i];
        }

        if (new_size > bound) {
            for (size_t i = bound; i < new_size; ++i) {
                newArray[i] = 0;
            }
        }

        delete[] arr;
        return newArray;
    }

    void copy(const int* src, int* dest, size_t srcSize, size_t srcPos, size_t destSize, size_t destPos, size_t count) {
        if (src == nullptr || dest == nullptr || srcPos >= srcSize || destPos >= destSize) {
            return;
        }
        size_t maxSrcAvailable = srcSize - srcPos;
        size_t maxDestAvailable = destSize - destPos;
        size_t resultCount = std::min({maxSrcAvailable, maxDestAvailable, count});

        for (size_t i = 0; i < resultCount; ++i) {
            dest[destPos + i] = src[srcPos + i];
        }
    }

    int* insert(int* arr, size_t& size, size_t pos, int value) {
        if (arr == nullptr) {
            return nullptr;
        }

        if (pos > size) {
            return arr;
        }

        int* newArray = new int[++size];
        copy(arr, newArray, size - 1, 0, size, 0, pos);
        newArray[pos] = value;
        copy(arr, newArray, size - 1, pos, size, pos + 1, size - 1 - pos);

        delete[] arr;
        return newArray;
    }

    int* remove(int* arr, size_t& size, size_t pos) {
        if (arr == nullptr || pos >= size) {
            return nullptr;
        }

        int* newArray = new int[--size];
        copy(arr, newArray, size + 1, 0, size, 0, pos);
        copy(arr, newArray, size + 1, pos + 1, size, pos, size - pos);

        delete[] arr;
        return newArray;
    }

    void print(const int* arr, size_t size) {
        std::cout << "Array: ";
        for (int i = 0; i < size; ++i) {
            std::cout << arr[i] << ' ';
        }
        std::cout << '\n';
    }

    bool contains(const int* arr, size_t size, int target, size_t& out_index) {
        out_index = std::numeric_limits<size_t>::max();
        if (arr == nullptr || size == 0) {
            return false;
        }

        size_t left = 0;
        size_t right = size;
        size_t middle = (left + right) / 2;

        while (left < right) {
            if (target > arr[middle]) {
                left = middle + 1;
            } else if (target < arr[middle]) {
                right = middle;
            } else {
                out_index = middle;
                return true;
            }
            middle = (left + right) / 2;
        }

        return false;
    }

    void fill(int* arr, size_t size, int lowerBound, int upperBound) {
        std::random_device random;
        std::mt19937 generator(5);
        std::uniform_int_distribution<> distribution(lowerBound, upperBound);

        for (size_t i = 0; i < size; ++i) {
            arr[i] = distribution(generator);
        }
    }

    inline void swap(int& first, int& b) noexcept {
        const int temp = first;
        first = b;
        b = temp;
    }

    void sort(int* arr, size_t size) {
        if (arr == nullptr || size <= 1) {
            return;
        }

        for (size_t i = 0; i < size - 1; ++i) {
            for (size_t j = i + 1; j < size; ++j) {
                if (arr[i] > arr[j]) {
                    swap(arr[i], arr[j]);
                }
            }
        }
    }

    size_t removeDuplicates(int* arr, size_t size) {
        if (arr == nullptr || size <= 1) {
            return 0;
        }

        size_t pos = 1;
        size_t prevPos = 0;
        size_t nextPos = 1;

        sort(arr, size);
        while (nextPos < size) {
            while (nextPos < size && arr[prevPos] == arr[nextPos]) {
                nextPos++;
            }
            if (arr[nextPos] == arr[prevPos]) {
                break;
            }
            arr[pos++] = arr[nextPos];
            prevPos = nextPos;
        }

        return pos - 1;
    }

}
