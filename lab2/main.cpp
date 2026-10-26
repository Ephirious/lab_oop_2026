#include "include/polynomial.h"
#include <iostream>

int main() {
    double c1[] = {1.0, 2.0};         // 1 + 2x
    double c2[] = {0.0, 1.0, 1.0};    // x + x^2
    Polynomial a(c1, 1);
    Polynomial b(c2, 2);

    std::cout << "A(x) with degree = " << a.getDegree() << ": " << a << '\n';
    std::cout << "B(x) with degree = " << b.getDegree() << ": " << b << '\n';


    Polynomial s(a);
    s.add(b);                         // 1 + 3x + x^2
    std::cout << "S(x) with degree = " << s.getDegree() << ": " << s << '\n';
    std::cout << s.evaluate(2.0) << std::endl; // 1 + 6 + 4 = 11

    Polynomial m(a);
    m.mul(b);                         // (1 + 2x)(x + x^2) = x + 3x^2 + 2x^3
    std::cout << "M(x) with degree = " << m.getDegree() << ": " << m << '\n';

    return 0;
}
