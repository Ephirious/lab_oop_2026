#ifndef LAB3_SET_H
#define LAB3_SET_H
#include <iosfwd>


namespace Collection {
    using std::size_t;

    class RGB {
        unsigned int red = 0;
        unsigned int green = 0;
        unsigned int blue = 0;

    public:
        RGB() = default;
        explicit RGB(unsigned int r, unsigned int g, unsigned int b);
        RGB(const RGB& source) = default;
        RGB(RGB&& source) noexcept = default;
        RGB& operator=(const RGB& source) = default;
        RGB& operator=(RGB&& source) noexcept = default;
        ~RGB() = default;

        friend bool operator==(const RGB& a, const RGB& b);
        friend bool operator!=(const RGB& a, const RGB& b);
        friend bool operator>(const RGB& a, const RGB& b);
        friend bool operator<(const RGB& a, const RGB& b);
        friend bool operator>=(const RGB& a, const RGB& b);
        friend bool operator<=(const RGB& a, const RGB& b);

        friend std::ostream& operator<<(std::ostream& os, const RGB& source);
    };


    class SetRGB {
        static constexpr size_t INCREASE_COEFFICIENT = 2;
        static constexpr size_t DEFAULT_CAPACITY = 32;

        RGB* elements;
        size_t collectionSize;
        size_t capacity;


        SetRGB(RGB* elements, size_t collectionSize, size_t capacity);

        inline void setZeroFields();
        size_t binarySearchLowerBound(const RGB& element) const;
        size_t binarySearchUpperBound(const RGB& element) const;

        inline friend void swap(SetRGB& a, SetRGB& b) noexcept;


    public:
        static constexpr size_t NPOS = static_cast<size_t>(-1);

        explicit SetRGB();
        explicit SetRGB(size_t size);
        SetRGB(const SetRGB& source);
        SetRGB(SetRGB&& source) noexcept;
        SetRGB& operator=(const SetRGB& source);
        SetRGB& operator=(SetRGB&& source) noexcept;
        ~SetRGB();

        bool add(const RGB& element);
        bool remove(const RGB& element);
        [[nodiscard]] bool contains(const RGB& element) const;
        [[nodiscard]] bool operator()(const RGB& element) const;

        [[nodiscard]] size_t size() const;
        [[nodiscard]] bool isEmpty() const;

        SetRGB& operator+=(const RGB& element);
        SetRGB& operator-=(const RGB& element);

        SetRGB operator|(const SetRGB& anySet) const;
        SetRGB operator&(const SetRGB& anySet) const;
        SetRGB operator-(const SetRGB& anySet) const;
        SetRGB operator^(const SetRGB& anySet) const;

        bool operator==(const SetRGB& anySet) const;
        bool operator!=(const SetRGB& anySet) const;

        friend std::ostream& operator<<(std::ostream& os, const SetRGB& element);

    };
}


#endif //LAB3_SET_H