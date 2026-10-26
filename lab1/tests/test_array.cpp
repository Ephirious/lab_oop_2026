#include <algorithm>
#include <gtest/gtest.h>
#include "../include/array.h"

TEST(ArrayCreationTest, CreateArray) {
    int* array = Array::create(10);
    EXPECT_NE(array, nullptr);
    Array::clear(array);
}

class ArrayTestFixture : public ::testing::Test {
protected:
    int* array = nullptr;
    size_t size = 0;

    void TearDown() override {
        if (array != nullptr) {
            Array::clear(array);
        }
    }

    bool isEqualArray(const std::initializer_list<int> expected) const {
        if (array == nullptr || expected.size() != size) {
            return false;
        }

        for (size_t i = 0; i < size; ++i) {
            if (array[i] != expected.begin()[i]) {
                return false;
            }
        }
        return true;
    }

    void initArray(const std::initializer_list<int> placeholder) {
        if (array != nullptr) {
            return;
        }
        size = placeholder.size();
        array = new int[size];
        Array::copy(placeholder.begin(), array, size, 0, size, 0, size);
    }
};

TEST_F(ArrayTestFixture, ArrayClear) {
    Array::clear(array);
    EXPECT_EQ(array, nullptr);
}

TEST_F(ArrayTestFixture, ResizeArray) {
    initArray({1, 2, 3, 4, 5});
    int newSize = 10;
    array = Array::resize(array, size, newSize);
    size = newSize;
    EXPECT_TRUE(isEqualArray({1, 2, 3, 4, 5, 0, 0, 0, 0, 0}));
}

TEST_F(ArrayTestFixture, ProvideNullptrWhenInsertIntoNullptrArray) {
    array = Array::insert(array, size, size + 1, 10);
    EXPECT_EQ(array, nullptr);
}

TEST_F(ArrayTestFixture, InsertAtBegining) {
    initArray({1, 2, 3, 4, 5});
    array = Array::insert(array, size, 0, 10);
    EXPECT_TRUE(isEqualArray({10, 1, 2, 3, 4, 5}));
}

TEST_F(ArrayTestFixture, InsertInMiddle) {
    initArray({1, 2, 3, 4, 5});
    array = Array::insert(array, size, 2, 10);
    EXPECT_TRUE(isEqualArray({1, 2, 10, 3, 4, 5}));
}

TEST_F(ArrayTestFixture, InsertInEnd) {
    initArray({1, 2, 3, 4, 5});
    array = Array::insert(array, size, 5, 10);
    EXPECT_TRUE(isEqualArray({1, 2, 3, 4, 5, 10}));
}

TEST_F(ArrayTestFixture, InsertOutOfBound) {
    initArray({1, 2, 3, 4, 5});
    array = Array::insert(array, size, size + 2, 10);
    EXPECT_TRUE(isEqualArray({1, 2, 3, 4, 5}));
}

TEST_F(ArrayTestFixture, ProvideNullptrWhenRemoveFromNullptrArray) {
    array = Array::remove(array, size, 0);
    EXPECT_EQ(array, nullptr);
}

TEST_F(ArrayTestFixture, RemoveFirstElement) {
    initArray({1, 2, 3, 4, 5});
    array = Array::remove(array, size, 0);
    EXPECT_TRUE(isEqualArray({2, 3, 4, 5}));
}

TEST_F(ArrayTestFixture, RemoveElementInMiddle) {
    initArray({1, 2, 3, 4, 5});
    array = Array::remove(array, size, 2);
    EXPECT_TRUE(isEqualArray({1, 2, 4, 5}));
}

TEST_F(ArrayTestFixture, RemoveLastElement) {
    initArray({1, 2, 3, 4, 5});
    array = Array::remove(array, size, size - 1);
    EXPECT_TRUE(isEqualArray({1, 2, 3, 4}));
}

TEST_F(ArrayTestFixture, ExpectContainsElement) {
    initArray({1, 2, 3, 4, 5});
    for (size_t i = 0; i < size; ++i) {
        size_t index;
        EXPECT_TRUE(Array::contains(array, size, array[i], index));
        EXPECT_EQ(i, index);
    }
}

TEST_F(ArrayTestFixture, ExpectNotContainsElement) {
    initArray({1, 2, 3, 4, 5});
    for (size_t i = 0; i < size; ++i) {
        size_t index;
        EXPECT_FALSE(Array::contains(array, size, array[i] + 10, index));
        EXPECT_EQ(index, std::numeric_limits<size_t>::max());
    }
}

TEST_F(ArrayTestFixture, SortNullprtArray) {
    Array::sort(array, size);
    EXPECT_EQ(array, nullptr);
}

TEST_F(ArrayTestFixture, SortOneElementArray) {
    initArray({1});
    Array::sort(array, size);
    EXPECT_EQ(array[0], 1);
}

TEST_F(ArrayTestFixture, SortInverseArray) {
    initArray({5, 4, 3, 2, 1});
    Array::sort(array, size);
    EXPECT_TRUE(isEqualArray({1, 2, 3, 4, 5}));
}

TEST_F(ArrayTestFixture, SortRandomArray) {
    int lowerBound = 10, upperBound = 100;
    Array::fill(array, size, lowerBound, upperBound);

    int* copyArray = Array::create(size);
    Array::copy(array, copyArray, size, 0, size, 0, size);

    Array::sort(copyArray, size);
    std::sort(copyArray, copyArray + size);

    for (size_t i = 0; i < size; ++i) {
        EXPECT_EQ(copyArray[i], array[i]);
    }
    Array::clear(copyArray);
}

TEST_F(ArrayTestFixture, RemoveDuplicatesWithoutDuplicates) {
    initArray({1, 2, 3, 4, 5});
    Array::removeDuplicates(array, size);
    EXPECT_TRUE(isEqualArray({1, 2, 3, 4, 5}));
}

TEST_F(ArrayTestFixture, RemoveDuplicateWithOneElement) {
    initArray({1});
    Array::removeDuplicates(array, size);
    EXPECT_TRUE(isEqualArray({1}));
}

TEST_F(ArrayTestFixture, RemoveSomeDuplicates) {
    initArray({1, 2, 2, 3, 4, 5, 5, 4, 3, 2, 3, 4, 5});
    size = Array::removeDuplicates(array, size);
    EXPECT_TRUE(isEqualArray({1, 2, 3, 4, 5}));
}
