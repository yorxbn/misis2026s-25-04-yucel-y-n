#include <complex/complex.hpp>
#include <cmath>
#include <limits>

Complex Complex::operator-() const noexcept {
  return Complex(-re, -im);
}

bool Complex::operator==(const Complex& rhs) const noexcept {
  const double eps = 2 * std::numeric_limits<double>::epsilon();
  return std::fabs(re - rhs.re) <= eps && std::fabs(im - rhs.im) <= eps;
}

bool Complex::operator!=(const Complex& rhs) const noexcept {
  return !(*this == rhs);
}

Complex& Complex::operator+=(const Complex& rhs) noexcept {
  re += rhs.re;
  im += rhs.im;
  return *this;
}

Complex& Complex::operator+=(const double rhs) noexcept {
  return operator+=(Complex(rhs));
}

Complex& Complex::operator-=(const Complex& rhs) noexcept {
  re -= rhs.re;
  im -= rhs.im;
  return *this;
}

Complex& Complex::operator-=(const double rhs) noexcept {
  return operator-=(Complex(rhs));
}

Complex& Complex::operator*=(const Complex& rhs) noexcept {
  double new_re = re * rhs.re - im * rhs.im;
  double new_im = re * rhs.im + im * rhs.re;
  re = new_re;
  im = new_im;
  return *this;
}

Complex& Complex::operator*=(const double rhs) noexcept {
  re *= rhs;
  im *= rhs;
  return *this;
}

Complex& Complex::operator/=(const Complex& rhs) {
  double denominator = rhs.re * rhs.re + rhs.im * rhs.im;
  if (denominator == 0.0) {
    re = std::numeric_limits<double>::infinity();
    im = std::numeric_limits<double>::infinity();
    return *this;
  }
  double new_re = (re * rhs.re + im * rhs.im) / denominator;
  double new_im = (im * rhs.re - re * rhs.im) / denominator;
  re = new_re;
  im = new_im;
  return *this;
}

Complex& Complex::operator/=(const double rhs) {
  if (rhs == 0.0) {
    re = std::numeric_limits<double>::infinity();
    im = std::numeric_limits<double>::infinity();
    return *this;
  }
  re /= rhs;
  im /= rhs;
  return *this;
}

std::ostream& Complex::WriteTo(std::ostream& ostrm) const noexcept {
  ostrm << leftBrace << re << separator << im << rightBrace;
  return ostrm;
}

std::istream& Complex::ReadFrom(std::istream& istrm) noexcept {
  char left, comma, right;
  double new_re, new_im;
  istrm >> left >> new_re >> comma >> new_im >> right;
  if (istrm.good() && left == leftBrace && comma == separator && right == rightBrace) {
    re = new_re;
    im = new_im;
  } else {
    istrm.setstate(std::ios::failbit);
  }
  return istrm;
}

Complex operator+(const Complex& lhs, const Complex& rhs) noexcept {
  return Complex(lhs) += rhs;
}

Complex operator+(const Complex& lhs, const double rhs) noexcept {
  return Complex(lhs) += rhs;
}

Complex operator+(const double lhs, const Complex& rhs) noexcept {
  return Complex(lhs) += rhs;
}

Complex operator-(const Complex& lhs, const Complex& rhs) noexcept {
  return Complex(lhs) -= rhs;
}

Complex operator-(const Complex& lhs, const double rhs) noexcept {
  return Complex(lhs) -= rhs;
}

Complex operator-(const double lhs, const Complex& rhs) noexcept {
  return Complex(lhs) -= rhs;
}

Complex operator*(const Complex& lhs, const Complex& rhs) noexcept {
  return Complex(lhs) *= rhs;
}

Complex operator*(const Complex& lhs, const double rhs) noexcept {
  return Complex(lhs) *= rhs;
}

Complex operator*(const double lhs, const Complex& rhs) noexcept {
  return Complex(lhs) *= rhs;
}

Complex operator/(const Complex& lhs, const Complex& rhs) {
  return Complex(lhs) /= rhs;
}

Complex operator/(const Complex& lhs, const double rhs) {
  return Complex(lhs) /= rhs;
}

Complex operator/(const double lhs, const Complex& rhs) {
  return Complex(lhs) /= rhs;
}
