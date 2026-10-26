#include "../include/collection.h"

#include <algorithm>
#include <stdexcept>
#include <iostream>

namespace Collection {
    bool isIncorrectColor(unsigned int color) {
        if (color > 255) {
            return true;
        }
        return false;
    }

    RGB::RGB(unsigned int r, unsigned int g, unsigned int b) {
        if (isIncorrectColor(r) || isIncorrectColor(g) || isIncorrectColor(b)) {
            throw std::invalid_argument("One of colors is invalid");
        }
        red = r;
        green = g;
        blue = b;
    }

    bool operator==(const RGB& a, const RGB& b) {
        return a.red == b.red && a.green == b.green && a.blue == b.blue;
    }

    bool operator!=(const RGB& a, const RGB& b) {
        return !(a == b);
    }

    bool operator>(const RGB& a, const RGB& b) {
        return a.red > b.red ? true : a.red < b.red ? false :
            a.green > b.green ? true : a.green < b.green ? false :
            a.blue > b.blue;
    }

    bool operator<(const RGB& a, const RGB& b) {
        return b > a;
    }
    bool operator>=(const RGB& a, const RGB& b) {
        return (a > b) || (a == b);
    }

    bool operator<=(const RGB& a, const RGB& b) {
        return !(a > b);
    }

    std::ostream& operator<<(std::ostream& os, const Collection::RGB& source) {
        os << "RGB(" << source.red << ", " << source.green << ", " << source.blue << ")";
        return os;
    }

    inline void swap(SetRGB& a, SetRGB& b) noexcept {
        std::swap(a.elements, b.elements);
        std::swap(a.collectionSize, b.collectionSize);
        std::swap(a.capacity, b.capacity);
    }


    inline void SetRGB::setZeroFields() {
        this->elements = nullptr;
        this->collectionSize = 0;
        this->capacity = 0;
    }

    SetRGB::SetRGB() : elements(nullptr), collectionSize(0), capacity(0) {}

    SetRGB::SetRGB(size_t size) :
        elements(new RGB[size == 0 ? DEFAULT_CAPACITY : size]),
        collectionSize(size),
        capacity(size == 0 ? DEFAULT_CAPACITY : size * INCREASE_COEFFICIENT) {}

    SetRGB::SetRGB(RGB* elements, size_t collectionSize, size_t capacity) :
        elements(elements), collectionSize(collectionSize), capacity(capacity) {}

    SetRGB::SetRGB(const SetRGB& source) : elements(new RGB[source.capacity]), collectionSize(source.collectionSize), capacity(source.capacity) {
        std::copy_n(source.elements, source.collectionSize, this->elements);
    }

    SetRGB::SetRGB(SetRGB&& source) noexcept {
        this->elements = source.elements;
        this->collectionSize = source.collectionSize;
        this->capacity = source.capacity;
        source.setZeroFields();
    }

    SetRGB& SetRGB::operator=(const SetRGB& source) {
        if (this != &source) {
            SetRGB copy(source);
            swap(*this, copy);
        }
        return *this;
    }

    SetRGB& SetRGB::operator=(SetRGB&& source) noexcept {
        if (this == &source) {
            return *this;
        }

        delete[] this->elements;

        this->elements = source.elements;
        this->collectionSize = source.collectionSize;
        this->capacity = source.capacity;

        source.setZeroFields();
        return *this;
    }

    SetRGB::~SetRGB() {
        delete[] this->elements;
        this->setZeroFields();
    }

    size_t SetRGB::binarySearchLowerBound(const RGB& element) const {
        size_t left = 0;
        size_t right = collectionSize;
        while (left < right) {
            size_t mid = left + (right - left) / 2;
            if (element > this->elements[mid]) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }

        return left;
    }

    size_t SetRGB::binarySearchUpperBound(const RGB& element) const {
        size_t left = 0;
        size_t right = collectionSize;
        while (left < right) {
            size_t mid = left + (right - left) / 2;
            if (element >= this->elements[mid]) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }

        return left;
    }



    bool SetRGB::add(const RGB& element) {
        size_t pos = binarySearchLowerBound(element);

        if (pos < collectionSize && elements[pos] == element) {
            return false;
        }

        if (collectionSize + 1 > capacity) {
            if (capacity * INCREASE_COEFFICIENT < capacity) {
                throw std::overflow_error("Capacity exceeded");
            }
            size_t newCapacity = capacity * INCREASE_COEFFICIENT;
            RGB* newElements = new RGB[newCapacity];

            std::copy_n(elements, pos, newElements);
            std::copy_n(elements + pos, collectionSize - pos, newElements + pos + 1);

            delete[] elements;
            elements = newElements;
            capacity = newCapacity;
        } else {
            std::copy_backward(elements + pos, elements + collectionSize, elements + pos + 1);
        }

        elements[pos] = element;
        ++collectionSize;

        return true;
    }

    bool SetRGB::remove(const RGB& element) {
        size_t pos = binarySearchLowerBound(element);

        if (pos == collectionSize || elements[pos] != element) {
            return false;
        }

        std::copy_n(elements + pos + 1, collectionSize - pos - 1, elements + pos);
        --collectionSize;
        return true;
    }

    bool SetRGB::contains(const RGB& element) const {
        size_t pos = binarySearchLowerBound(element);

        if (pos == collectionSize || elements[pos] != element) {
            return false;
        }

        return true;
    }

    bool SetRGB::operator()(const RGB& element) const {
        return contains(element);
    }

    size_t SetRGB::size() const {
        return collectionSize;
    }

    bool SetRGB::isEmpty() const {
        return collectionSize == 0;
    }

    SetRGB& SetRGB::operator+=(const RGB& element) {
        add(element);
        return *this;
    }

    SetRGB& SetRGB::operator-=(const RGB& element) {
        remove(element);
        return *this;
    }

    SetRGB SetRGB::operator|(const SetRGB& anySet) const {
        if (collectionSize == 0) {
            return {anySet};
        }
        if (anySet.collectionSize == 0) {
            return {*this};
        }

        size_t tempSize = collectionSize + anySet.collectionSize;
        RGB* newElements = new RGB[tempSize];

        size_t left = 0;
        size_t right = 0;
        size_t resultIndex = 0;

        while (left < this->collectionSize && right < anySet.collectionSize) {
            if (elements[left] < anySet.elements[right]) {
                newElements[resultIndex++] = elements[left++];
            } else if (anySet.elements[right] < elements[left]) {
                newElements[resultIndex++] = anySet.elements[right++];
            } else {
                newElements[resultIndex++] = elements[left++];
                ++right;
            }
        }

        while (left < collectionSize) {
            newElements[resultIndex++] = elements[left++];
        }

        while (right < anySet.collectionSize) {
            newElements[resultIndex++] = anySet.elements[right++];
        }

        return {newElements, resultIndex, tempSize};
    }

    SetRGB SetRGB::operator&(const SetRGB& anySet) const {
        if (collectionSize == 0 || anySet.collectionSize == 0) {
            return SetRGB();
        }

        size_t tempSize = std::min(collectionSize, anySet.collectionSize);
        RGB* newElements = new RGB[tempSize];

        size_t left = 0;
        size_t right = 0;
        size_t resultIndex = 0;

        while (left < collectionSize && right < anySet.collectionSize) {
            if (elements[left] < anySet.elements[right]) {
                ++left;
            }
            if (anySet.elements[right] < elements[left]) {
                ++right;
            }

            if (elements[left] == anySet.elements[right]) {
                newElements[resultIndex++] = elements[left++];
                right++;
            }
        }

        return {newElements, resultIndex, tempSize};
    }

    SetRGB SetRGB::operator-(const SetRGB& anySet) const {
        if (this->collectionSize == 0) {
            return SetRGB{};
        }
        if (anySet.collectionSize == 0) {
            return {*this};
        }

        size_t tempSize = collectionSize;
        RGB* newElements = new RGB[tempSize];

        size_t left = 0;
        size_t right = 0;
        size_t resultIndex = 0;

        while (left < collectionSize && right < anySet.collectionSize) {
            if (elements[left] < anySet.elements[right]) {
                newElements[resultIndex++] = elements[left++];
            } else if (elements[left] > anySet.elements[right]) {
                ++right;
            } else {
                ++right;
                ++left;
            }
        }

        while (left < collectionSize) {
            newElements[resultIndex++] = elements[left++];
        }

        return {newElements, resultIndex, tempSize};
    }

    SetRGB SetRGB::operator^(const SetRGB& anySet) const {
        if (this->collectionSize == 0) {
            return {anySet};
        }
        if (anySet.collectionSize == 0) {
            return {*this};
        }

        size_t tempSize = collectionSize + anySet.collectionSize;
        RGB* newElements = new RGB[tempSize];

        size_t left = 0;
        size_t right = 0;
        size_t resultIndex = 0;

        while (left < collectionSize && right < anySet.collectionSize) {
            if (elements[left] < anySet.elements[right]) {
                newElements[resultIndex++] = elements[left++];
            } else if (elements[left] > anySet.elements[right]) {
                newElements[resultIndex++] = anySet.elements[right++];
            } else {
                ++right;
                ++left;
            }
        }

        while (left < collectionSize) {
            newElements[resultIndex++] = elements[left++];
        }

        while (right < anySet.collectionSize) {
            newElements[resultIndex++] = anySet.elements[right++];
        }

        return {newElements, resultIndex, tempSize};
    }

    bool SetRGB::operator==(const SetRGB& anySet) const {
        if (collectionSize != anySet.collectionSize) {
            return false;
        }

        for (size_t i = 0; i < collectionSize; ++i) {
            if (elements[i] != anySet.elements[i]) {
                return false;
            }
        }
        return true;
    }

    bool SetRGB::operator!=(const SetRGB& anySet) const {
        return !(*this == anySet);
    }

    std::ostream& operator<<(std::ostream& os, const SetRGB& element) {
        os << "RGB{";
        for (size_t i = 0; i < element.collectionSize - 1; ++i) {
            os << element.elements[i] << ", ";
        }
        os << element.elements[element.collectionSize - 1] << "}";

        return os;
    }
}
