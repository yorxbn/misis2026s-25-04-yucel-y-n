#include "complex.hpp"
#include <stdexcept>

Complex::Complex(const double real)
    : re(real)
    , im(0.0)
{
}

Complex::Complex(const double real, const double imaginary)
    : re(real)
    , im(imaginary)
{
}

Complex& Complex::operator+=(const Complex& rhs) {
    re += rhs.re;
    im += rhs.im;
    return *this;
}

Complex& Complex::operator+=(const double rhs) {
    re += rhs;
    return *this;
}

Complex& Complex::operator-=(const Complex& rhs) {
    re -= rhs.re;
    im -= rhs.im;
    return *this;
}

Complex& Complex::operator-=(const double rhs) {
    re -= rhs;
    return *this;
}

Complex& Complex::operator*=(const Complex& rhs) {
    const double oldRe = re;
    const double oldIm = im;
    re = oldRe * rhs.re - oldIm * rhs.im;
    im = oldRe * rhs.im + oldIm * rhs.re;
    return *this;
}

Complex& Complex::operator*=(const double rhs) {
    re *= rhs;
    im *= rhs;
    return *this;
}

Complex& Complex::operator/=(const Complex& rhs) {
    const double denominator = rhs.re * rhs.re + rhs.im * rhs.im;
    if (denominator == 0.0) {
        throw std::runtime_error("division by zero");
    }
    const double oldRe = re;
    const double oldIm = im;
    re = (oldRe * rhs.re + oldIm * rhs.im) / denominator;
    im = (oldIm * rhs.re - oldRe * rhs.im) / denominator;
    return *this;
}

Complex& Complex::operator/=(const double rhs) {
    if (rhs == 0.0) {
        throw std::runtime_error("division by zero");
    }
    re /= rhs;
    im /= rhs;
    return *this;
}

std::ostream& Complex::writeTo(std::ostream& ostrm) const {
    ostrm << leftBrace << re << separator << im << rightBrace;
    return ostrm;
}

std::istream& Complex::readFrom(std::istream& istrm) {
    char leftBrace0 = 0;
    double real0 = 0.0;
    char comma0 = 0;
    double imag0 = 0.0;
    char rightBrace0 = 0;

    istrm >> leftBrace0 >> real0 >> comma0 >> imag0 >> rightBrace0;
    if (istrm.good()) {
        if (leftBrace == leftBrace0 && separator == comma0 && rightBrace == rightBrace0) {
            re = real0;
            im = imag0;
        }
        else {
            istrm.setstate(std::ios_base::failbit);
        }
    }
    return istrm;
}

Complex Complex::operator-() const noexcept {
    return Complex(-re, -im);
}


Complex operator+(const Complex& lhs, const Complex& rhs) {
    Complex res(lhs);
    res += rhs;
    return res;
}

Complex operator+(const Complex& lhs, const double rhs) {
    Complex res(lhs);
    res += rhs;
    return res;
}

Complex operator+(const double lhs, const Complex& rhs) {
    Complex res(lhs);
    res += rhs;
    return res;
}

Complex operator-(const Complex& lhs, const Complex& rhs) {
    Complex res(lhs);
    res -= rhs;
    return res;
}

Complex operator-(const Complex& lhs, const double rhs) {
    Complex res(lhs);
    res -= rhs;
    return res;
}

Complex operator-(const double lhs, const Complex& rhs) {
    Complex res(lhs);
    res -= rhs;
    return res;
}

Complex operator*(const Complex& lhs, const Complex& rhs) {
    Complex res(lhs);
    res *= rhs;
    return res;
}

Complex operator*(const Complex& lhs, const double rhs) {
    Complex res(lhs);
    res *= rhs;
    return res;
}

Complex operator*(const double lhs, const Complex& rhs) {
    Complex res(rhs);
    res *= lhs;
    return res;
}

Complex operator/(const Complex& lhs, const Complex& rhs) {
    Complex res(lhs);
    res /= rhs;
    return res;
}

Complex operator/(const Complex& lhs, const double rhs) {
    Complex res(lhs);
    res /= rhs;
    return res;
}

Complex operator/(const double lhs, const Complex& rhs) {
    Complex res(lhs);
    res /= rhs;
    return res;
}
