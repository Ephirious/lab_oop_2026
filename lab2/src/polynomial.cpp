#include "../include/polynomial.h"

#include <algorithm>
#include <cmath>
#include <format>
#include <iostream>

inline void printMonomial(std::ostream& os, double coefficient, int degree, bool& isFirstMonomial) {
    constexpr auto epsilon = 1e-7;
    if (std::abs(coefficient) < epsilon) {
        return;
    }

    if (coefficient > epsilon) {
        if (!isFirstMonomial) {
            os << " + ";
        }
    } else {
        os << ((isFirstMonomial) ? "-" : " - ");
        coefficient = -coefficient;
    }

    if (degree == 0 || std::abs(1 - coefficient) > epsilon) {
        os << coefficient;
    }

    if (degree == 1) {
        os << 'x';
    } else if (degree > 1) {
        os << "x^" << degree;
    }

    isFirstMonomial = false;
}

void swap(Polynomial& a, Polynomial& b) noexcept {
    std::swap(a.coefficients, b.coefficients);
    std::swap(a.degree, b.degree);
}

std::ostream& operator<<(std::ostream& os, const Polynomial& polynomial) {
    bool isFirstMonomial = true;
    printMonomial(os, polynomial.coefficients[0], 0, isFirstMonomial);
    for (std::size_t i = 1; i < polynomial.degree + 1; ++i) {
        printMonomial(os, polynomial.coefficients[i], static_cast<int>(i), isFirstMonomial);
    }
    return os;
}

Polynomial::Polynomial(const double* coefficients, int degree) {
    if (coefficients == nullptr || degree < 0) {
        return;
    }

    this->degree = degree;
    this->coefficients = new double[degree + 1];
    std::copy_n(coefficients, degree + 1, this->coefficients);
}

Polynomial::Polynomial(const Polynomial& source)
    : degree(source.degree),
    coefficients(new double[source.degree + 1])
{
    std::copy_n(source.coefficients, degree + 1, this->coefficients);
}

Polynomial::Polynomial(Polynomial&& source) noexcept
    : degree(source.degree),
    coefficients(source.coefficients)
{
    source.degree = -1;
    source.coefficients = nullptr;
}

Polynomial& Polynomial::operator=(const Polynomial& source) {
    if (this != &source) {
        Polynomial temp(source);
        swap(temp, *this);
    }
    return *this;
}

Polynomial& Polynomial::operator=(Polynomial&& source) noexcept {
    if (this == &source) {
        return *this;
    }
    delete[] this->coefficients;
    this->coefficients = source.coefficients;
    this->degree = source.degree;

    source.degree = -1;
    source.coefficients = nullptr;

    return *this;
}

Polynomial::~Polynomial() {
    delete[] this->coefficients;
    coefficients = nullptr;
    degree = -1;
}


int Polynomial::getDegree() const {
    return degree;
}

const double* Polynomial::getCoefficients() const {
    return coefficients;
}

double Polynomial::sumCoefficients() const {
    double sum = 0;
    for (int i = 0; i < degree + 1; ++i) {
        sum += this->coefficients[i];
    }
    return sum;
}

bool Polynomial::isValidPolynomialInvariant() const {
    if (degree == -1 || coefficients == nullptr) {
        return false;
    }
    return true;
}


void Polynomial::add(const Polynomial& source) {
    if (!this->isValidPolynomialInvariant() || !source.isValidPolynomialInvariant()) {
        throw std::invalid_argument("Polynomial::add called with invalid input");
    }

    if (this->degree >= source.degree) {
        for (std::size_t i = 0; i < source.degree + 1; ++i) {
            this->coefficients[i] += source.coefficients[i];
        }
    } else {
        Polynomial temp(source);
        for (std::size_t i = 0; i < this->degree + 1; ++i) {
            temp.coefficients[i] += this->coefficients[i];
        }
        *this = std::move(temp);
    }
}

double Polynomial::evaluate(double x) const {
    if (!this->isValidPolynomialInvariant()) {
        throw std::invalid_argument("Polynomial::evaluate called with invalid input");
    }
    double sum = coefficients[0];
    for (std::size_t i = 1; i < this->degree + 1; ++i) {
        sum += this->coefficients[i] * std::pow(x, static_cast<int>(i));
    }

    return sum;
}

void Polynomial::mul(const Polynomial& source) {
    int resultDegree = this->degree + source.degree;
    double* resultCoefficients = new double[resultDegree + 1]();


    for (std::size_t i = 0; i < this->degree + 1; ++i) {
        for (std::size_t j = 0; j < source.degree + 1; ++j) {
            resultCoefficients[i + j] += source.coefficients[j] * this->coefficients[i];
        }
    }
    *this = Polynomial(resultCoefficients, resultDegree);
    delete[] resultCoefficients;
}
