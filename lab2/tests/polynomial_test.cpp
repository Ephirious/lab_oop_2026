#include <gtest/gtest.h>
#include <cmath>
#include <sstream>
#include <initializer_list>
#include <vector>
#include "../include/polynomial.h"

TEST(PolynomialCreationTest, CreateValidPolynomial) {
    double data[] = {1.0, 2.0, 3.0};
    Polynomial poly(data, 2);

    EXPECT_TRUE(poly.isValidPolynomialInvariant());
    EXPECT_EQ(poly.getDegree(), 2);
    ASSERT_NE(poly.getCoefficients(), nullptr);
    EXPECT_DOUBLE_EQ(poly.getCoefficients()[0], 1.0);
    EXPECT_DOUBLE_EQ(poly.getCoefficients()[1], 2.0);
    EXPECT_DOUBLE_EQ(poly.getCoefficients()[2], 3.0);
}

TEST(PolynomialCreationTest, CreateWithNullptr) {
    Polynomial poly(nullptr, 2);
    EXPECT_FALSE(poly.isValidPolynomialInvariant());
    EXPECT_EQ(poly.getDegree(), -1);
    EXPECT_EQ(poly.getCoefficients(), nullptr);
}

TEST(PolynomialCreationTest, CreateWithNegativeDegree) {
    double data[] = {1.0, 2.0};
    Polynomial poly(data, -1);
    EXPECT_FALSE(poly.isValidPolynomialInvariant());
    EXPECT_EQ(poly.getDegree(), -1);
    EXPECT_EQ(poly.getCoefficients(), nullptr);
}

class PolynomialTestFixture : public ::testing::Test {
protected:
    Polynomial* poly = nullptr;

    void TearDown() override {
        delete poly;
        poly = nullptr;
    }

    void initPolynomial(const std::initializer_list<double> placeholder) {
        delete poly;
        int degree = static_cast<int>(placeholder.size()) - 1;
        std::vector<double> coeffs(placeholder);
        poly = new Polynomial(coeffs.data(), degree);
    }

    bool isEqualCoefficients(const std::initializer_list<double> expected, double epsilon = 1e-6) const {
        if (poly == nullptr || !poly->isValidPolynomialInvariant()) {
            return false;
        }

        int expectedDegree = static_cast<int>(expected.size()) - 1;
        if (poly->getDegree() != expectedDegree) {
            return false;
        }

        const double* actualCoeffs = poly->getCoefficients();
        if (actualCoeffs == nullptr) {
            return false;
        }

        size_t i = 0;
        for (double val : expected) {
            if (std::abs(actualCoeffs[i++] - val) > epsilon) {
                return false;
            }
        }
        return true;
    }
};

TEST_F(PolynomialTestFixture, InvariantValidation) {
    initPolynomial({1.0, 2.0, 3.0});
    EXPECT_TRUE(poly->isValidPolynomialInvariant());
}

TEST_F(PolynomialTestFixture, CopyConstructor) {
    initPolynomial({1.0, -2.0, 3.5});
    Polynomial copy(*poly);

    EXPECT_EQ(copy.getDegree(), poly->getDegree());
    EXPECT_NE(copy.getCoefficients(), poly->getCoefficients());
    EXPECT_TRUE(isEqualCoefficients({1.0, -2.0, 3.5}));
}

TEST_F(PolynomialTestFixture, CopyAssignment) {
    initPolynomial({2.0, 4.0, 6.0});
    double otherData[] = {1.0};
    Polynomial other(otherData, 0);

    other = *poly;

    EXPECT_EQ(other.getDegree(), 2);
    EXPECT_NE(other.getCoefficients(), poly->getCoefficients());
    EXPECT_DOUBLE_EQ(other.getCoefficients()[0], 2.0);
    EXPECT_DOUBLE_EQ(other.getCoefficients()[1], 4.0);
    EXPECT_DOUBLE_EQ(other.getCoefficients()[2], 6.0);
}

TEST_F(PolynomialTestFixture, MoveConstructor) {
    initPolynomial({1.0, 2.0, 3.0});
    Polynomial moved(std::move(*poly));

    EXPECT_EQ(moved.getDegree(), 2);
    EXPECT_FALSE(poly->isValidPolynomialInvariant());
    EXPECT_EQ(poly->getDegree(), -1);
    EXPECT_EQ(poly->getCoefficients(), nullptr);
}

TEST_F(PolynomialTestFixture, MoveAssignment) {
    initPolynomial({1.0, 2.0, 3.0});
    double otherData[] = {5.0};
    Polynomial destination(otherData, 0);

    destination = std::move(*poly);

    EXPECT_EQ(destination.getDegree(), 2);
    EXPECT_DOUBLE_EQ(destination.getCoefficients()[2], 3.0);
    EXPECT_FALSE(poly->isValidPolynomialInvariant());
    EXPECT_EQ(poly->getDegree(), -1);
    EXPECT_EQ(poly->getCoefficients(), nullptr);
}

TEST_F(PolynomialTestFixture, SumCoefficients) {
    initPolynomial({1.0, 2.0, 3.0});
    EXPECT_DOUBLE_EQ(poly->sumCoefficients(), 6.0);
}

TEST_F(PolynomialTestFixture, EvaluateValidPolynomial) {
    initPolynomial({1.0, 2.0, 3.0});
    EXPECT_DOUBLE_EQ(poly->evaluate(2.0), 17.0);
    EXPECT_DOUBLE_EQ(poly->evaluate(0.0), 1.0);
}

TEST_F(PolynomialTestFixture, EvaluateThrowsOnInvalid) {
    Polynomial invalidPoly(nullptr, 0);
    EXPECT_THROW(invalidPoly.evaluate(1.0), std::invalid_argument);
}

TEST_F(PolynomialTestFixture, AddSameDegree) {
    initPolynomial({1.0, 2.0, 3.0});
    double otherData[] = {4.0, 5.0, 6.0};
    Polynomial other(otherData, 2);

    poly->add(other);

    EXPECT_TRUE(isEqualCoefficients({5.0, 7.0, 9.0}));
}

TEST_F(PolynomialTestFixture, AddLowerDegree) {
    initPolynomial({1.0, 2.0, 3.0});
    double otherData[] = {5.0, 5.0};
    Polynomial other(otherData, 1);

    poly->add(other);

    EXPECT_TRUE(isEqualCoefficients({6.0, 7.0, 3.0}));
}

TEST_F(PolynomialTestFixture, AddHigherDegree) {
    initPolynomial({1.0, 2.0});
    double otherData[] = {3.0, 4.0, 5.0};
    Polynomial other(otherData, 2);

    poly->add(other);

    EXPECT_TRUE(isEqualCoefficients({4.0, 6.0, 5.0}));
}

TEST_F(PolynomialTestFixture, AddThrowsOnInvalid) {
    initPolynomial({1.0, 2.0});
    Polynomial invalidPoly(nullptr, 0);

    EXPECT_THROW(poly->add(invalidPoly), std::invalid_argument);
    EXPECT_THROW(invalidPoly.add(*poly), std::invalid_argument);
}

TEST_F(PolynomialTestFixture, MultiplyPolynomials) {
    initPolynomial({1.0, 2.0});
    double otherData[] = {3.0, 4.0};
    Polynomial other(otherData, 1);

    poly->mul(other);

    EXPECT_TRUE(isEqualCoefficients({3.0, 10.0, 8.0}));
}

TEST_F(PolynomialTestFixture, MultiplyByConstant) {
    initPolynomial({2.0, 3.0, 4.0});
    double otherData[] = {2.0};
    Polynomial other(otherData, 0);

    poly->mul(other);

    EXPECT_TRUE(isEqualCoefficients({4.0, 6.0, 8.0}));
}

TEST_F(PolynomialTestFixture, StreamOutputFormatting) {
    initPolynomial({-1.0, 1.0, -2.0, 3.0});
    std::ostringstream oss;
    oss << *poly;

    EXPECT_EQ(oss.str(), "-1 + x - 2x^2 + 3x^3");
}

TEST_F(PolynomialTestFixture, StreamOutputZeroCoefficientsSkipped) {
    initPolynomial({0.0, 2.0, 0.0, 4.0});
    std::ostringstream oss;
    oss << *poly;

    EXPECT_EQ(oss.str(), "2x + 4x^3");
}