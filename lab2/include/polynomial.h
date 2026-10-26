#ifndef LAB2_POLYNOMIAL_H
#define LAB2_POLYNOMIAL_H
#include <iosfwd>


class Polynomial {
private:
    int degree = -1;
    double* coefficients = nullptr;

public:
    explicit Polynomial(const double* coefficients, int degree);
    Polynomial(const Polynomial& source);
    Polynomial(Polynomial&& source) noexcept;
    Polynomial& operator=(const Polynomial& source);
    Polynomial& operator=(Polynomial&& source) noexcept;
    ~Polynomial();

    [[nodiscard]] bool isValidPolynomialInvariant() const;

    [[nodiscard]] int getDegree() const;
    [[nodiscard]] const double* getCoefficients() const;
    [[nodiscard]] double sumCoefficients() const;
    [[nodiscard]] double evaluate(double x) const;

    void add(const Polynomial& source);
    void mul(const Polynomial& source);


    friend std::ostream& operator<<(std::ostream& os, const Polynomial& polynomial);
    friend void swap(Polynomial& a, Polynomial& b) noexcept;
};


#endif //LAB2_POLYNOMIAL_H