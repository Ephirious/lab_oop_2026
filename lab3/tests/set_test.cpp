#include <gtest/gtest.h>
#include <sstream>
#include <initializer_list>
#include "../include/collection.h"

using Collection::RGB;
using Collection::SetRGB;


TEST(RGBCreationTest, CreateValidRGB) {
    EXPECT_NO_THROW({
        RGB color(0, 128, 255);
    });
}

TEST(RGBCreationTest, ThrowOnInvalidColor) {
    EXPECT_THROW(RGB(256, 0, 0), std::invalid_argument);
    EXPECT_THROW(RGB(0, 300, 0), std::invalid_argument);
    EXPECT_THROW(RGB(0, 0, 1000), std::invalid_argument);
}

TEST(RGBComparisonTest, ComparisonOperators) {
    RGB c1(10, 20, 30);
    RGB c2(10, 20, 30);
    RGB c3(10, 20, 40);
    RGB c4(10, 30, 10);
    RGB c5(20, 10, 10);

    EXPECT_TRUE(c1 == c2);
    EXPECT_FALSE(c1 != c2);

    EXPECT_TRUE(c1 < c3);
    EXPECT_TRUE(c3 < c4);
    EXPECT_TRUE(c4 < c5);

    EXPECT_TRUE(c5 > c4);
    EXPECT_TRUE(c1 <= c2);
    EXPECT_TRUE(c1 >= c2);
    EXPECT_TRUE(c3 <= c4);
}

class
SetRGBTestFixture : public ::testing::Test {
protected:
    SetRGB* set = nullptr;

    void TearDown() override {
        delete set;
        set = nullptr;
    }

    void initSet(const std::initializer_list<RGB>& placeholder) {
        delete set;
        set = new SetRGB(0);
        for (const auto& item : placeholder) {
            set->add(item);
        }
    }
};

TEST_F(SetRGBTestFixture, Requirement1_EmptySetProperties) {
    initSet({});
    EXPECT_EQ(set->size(), 0);
    EXPECT_TRUE(set->isEmpty());
    EXPECT_FALSE(set->contains(RGB(10, 20, 30)));

    SetRGB defaultConstructed;
    EXPECT_EQ(defaultConstructed.size(), 0);
    EXPECT_TRUE(defaultConstructed.isEmpty());
    EXPECT_FALSE(defaultConstructed.contains(RGB(0, 0, 0)));
}

TEST_F(SetRGBTestFixture, Requirement2_AddNewElementAndDuplicate) {
    initSet({});
    RGB color(100, 150, 200);

    EXPECT_TRUE(set->add(color));
    EXPECT_EQ(set->size(), 1);
    EXPECT_TRUE(set->contains(color));

    EXPECT_FALSE(set->add(color));
    EXPECT_EQ(set->size(), 1);
}

TEST_F(SetRGBTestFixture, Requirement3_RemoveExistingAndNonExisting) {
    RGB c1(10, 20, 30);
    RGB c2(40, 50, 60);
    initSet({c1});

    EXPECT_FALSE(set->remove(c2));
    EXPECT_EQ(set->size(), 1);

    EXPECT_TRUE(set->remove(c1));
    EXPECT_EQ(set->size(), 0);
    EXPECT_FALSE(set->contains(c1));

    EXPECT_FALSE(set->remove(c1));
}

TEST_F(SetRGBTestFixture, Requirement4_ContainsAndCallOperatorConsistency) {
    RGB present(10, 20, 30);
    RGB absent(40, 50, 60);
    initSet({present});

    EXPECT_TRUE(set->contains(present));
    EXPECT_TRUE((*set)(present));
    EXPECT_EQ(set->contains(present), (*set)(present));

    EXPECT_FALSE(set->contains(absent));
    EXPECT_FALSE((*set)(absent));
    EXPECT_EQ(set->contains(absent), (*set)(absent));
}

TEST_F(SetRGBTestFixture, Requirement5_CopyIsIndependent) {
    RGB c1(10, 20, 30);
    RGB c2(40, 50, 60);
    RGB c3(70, 80, 90);
    initSet({c1, c2});

    SetRGB copy(*set);

    EXPECT_TRUE(copy.add(c3));
    EXPECT_TRUE(copy.contains(c3));
    EXPECT_FALSE(set->contains(c3));
    EXPECT_EQ(set->size(), 2);
    EXPECT_EQ(copy.size(), 3);

    EXPECT_TRUE(copy.remove(c1));
    EXPECT_FALSE(copy.contains(c1));
    EXPECT_TRUE(set->contains(c1));
    EXPECT_EQ(set->size(), 2);

    SetRGB copyAssigned(0);
    copyAssigned = *set;
    EXPECT_TRUE(copyAssigned.remove(c2));
    EXPECT_TRUE(set->contains(c2));
}

TEST_F(SetRGBTestFixture, Requirement6_MoveLeavesSourceInValidEmptyState) {
    RGB c1(10, 20, 30);
    RGB c2(40, 50, 60);
    initSet({c1, c2});

    SetRGB moved(std::move(*set));

    EXPECT_EQ(moved.size(), 2);
    EXPECT_TRUE(moved.contains(c1));
    EXPECT_TRUE(moved.contains(c2));

    EXPECT_EQ(set->size(), 0);
    EXPECT_TRUE(set->isEmpty());
    EXPECT_FALSE(set->contains(c1));
    EXPECT_FALSE(set->contains(c2));

    // Проверка перемещающего присваивания
    SetRGB destination(0);
    destination = std::move(moved);

    EXPECT_EQ(destination.size(), 2);
    EXPECT_EQ(moved.size(), 0);
    EXPECT_TRUE(moved.isEmpty());
}

TEST(SetRGBOperationsTest, Requirement7_SetOperationsUnionIntersectionDifference) {
    RGB c1(10, 0, 0);
    RGB c2(20, 0, 0);
    RGB c3(30, 0, 0);
    RGB c4(40, 0, 0);

    SetRGB a(0);
    a.add(c1);
    a.add(c2);
    a.add(c3); // A = {c1, c2, c3}

    SetRGB b(0);
    b.add(c2);
    b.add(c3);
    b.add(c4); // B = {c2, c3, c4}

    // Объединение: A | B = {c1, c2, c3, c4}
    SetRGB unionSet = a | b;
    EXPECT_EQ(unionSet.size(), 4);
    EXPECT_TRUE(unionSet.contains(c1));
    EXPECT_TRUE(unionSet.contains(c2));
    EXPECT_TRUE(unionSet.contains(c3));
    EXPECT_TRUE(unionSet.contains(c4));

    // Пересечение: A & B = {c2, c3}
    SetRGB intersectSet = a & b;
    EXPECT_EQ(intersectSet.size(), 2);
    EXPECT_FALSE(intersectSet.contains(c1));
    EXPECT_TRUE(intersectSet.contains(c2));
    EXPECT_TRUE(intersectSet.contains(c3));
    EXPECT_FALSE(intersectSet.contains(c4));

    // Разность: A - B = {c1}
    SetRGB diffSet = a - b;
    EXPECT_EQ(diffSet.size(), 1);
    EXPECT_TRUE(diffSet.contains(c1));
    EXPECT_FALSE(diffSet.contains(c2));
    EXPECT_FALSE(diffSet.contains(c3));
    EXPECT_FALSE(diffSet.contains(c4));

    // Симметрическая разность: A ^ B = {c1, c4}
    SetRGB symDiffSet = a ^ b;
    EXPECT_EQ(symDiffSet.size(), 2);
    EXPECT_TRUE(symDiffSet.contains(c1));
    EXPECT_FALSE(symDiffSet.contains(c2));
    EXPECT_FALSE(symDiffSet.contains(c3));
    EXPECT_TRUE(symDiffSet.contains(c4));
}

TEST_F(SetRGBTestFixture, Requirement8_EqualityAndInequalityConsistency) {
    RGB c1(10, 20, 30);
    RGB c2(40, 50, 60);
    initSet({c1, c2});

    SetRGB same(0);
    same.add(c1);
    same.add(c2);

    EXPECT_TRUE(*set == same);
    EXPECT_FALSE(*set != same);

    same.add(RGB(70, 80, 90));
    EXPECT_FALSE(*set == same);
    EXPECT_TRUE(*set != same);

    SetRGB different(0);
    different.add(c1);
    different.add(RGB(99, 99, 99));
    EXPECT_FALSE(*set == different);
    EXPECT_TRUE(*set != different);
}

TEST_F(SetRGBTestFixture, Requirement9_StreamOutputFormatting) {
    RGB c(255, 128, 0);
    std::ostringstream ossRgb;
    ossRgb << c;
    EXPECT_EQ(ossRgb.str(), "RGB(255, 128, 0)");

    initSet({RGB(10, 20, 30), RGB(40, 50, 60)});
    std::ostringstream ossSet;
    ossSet << *set;
    EXPECT_EQ(ossSet.str(), "RGB{RGB(10, 20, 30), RGB(40, 50, 60)}");
}

TEST(SetRGBStressTest, Requirement10_Insert1000ElementsAndCheckNoDuplicates) {
    SetRGB set(0);
    const size_t TOTAL_ELEMENTS = 1000;

    for (size_t i = 0; i < TOTAL_ELEMENTS; ++i) {
        unsigned int r = static_cast<unsigned int>(i / (256 * 256));
        unsigned int g = static_cast<unsigned int>((i / 256) % 256);
        unsigned int b = static_cast<unsigned int>(i % 256);
        EXPECT_TRUE(set.add(RGB(r, g, b)));
    }

    EXPECT_EQ(set.size(), TOTAL_ELEMENTS);

    for (size_t i = 0; i < TOTAL_ELEMENTS; ++i) {
        unsigned int r = static_cast<unsigned int>(i / (256 * 256));
        unsigned int g = static_cast<unsigned int>((i / 256) % 256);
        unsigned int b = static_cast<unsigned int>(i % 256);
        EXPECT_FALSE(set.add(RGB(r, g, b)));
    }

    EXPECT_EQ(set.size(), TOTAL_ELEMENTS);

    for (size_t i = 0; i < TOTAL_ELEMENTS; ++i) {
        unsigned int r = static_cast<unsigned int>(i / (256 * 256));
        unsigned int g = static_cast<unsigned int>((i / 256) % 256);
        unsigned int b = static_cast<unsigned int>(i % 256);
        EXPECT_TRUE(set.contains(RGB(r, g, b)));
    }
}

TEST_F(SetRGBTestFixture, CompoundAssignmentOperators) {
    RGB c1(10, 20, 30);
    initSet({});

    *set += c1;
    EXPECT_EQ(set->size(), 1);
    EXPECT_TRUE(set->contains(c1));

    *set -= c1;
    EXPECT_EQ(set->size(), 0);
    EXPECT_FALSE(set->contains(c1));
}