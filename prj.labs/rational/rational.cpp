#include "rational/rational.hpp"
#include <numeric>
#include <stdexcept>

Rational::Rational(const std::int32_t num, const std::int32_t den)
    : num_(num)
    , den_(den) {
    if (den_ == 0) {
        throw std::invalid_argument("Zero denumenator in Rational ctor");
    }
    Normalize();
}

void Rational::Normalize() noexcept {
    if (den_ < 0) {
        den_ = -den_;
        num_ = -num_;
    }

    if (num_ == 0) {
        den_ = 1;
        return;
    }

    const std::int32_t g = std::gcd(num_, den_);
    num_ /= g;
    den_ /= g;
}

bool Rational::operator==(const Rational& rhs) const noexcept {
    return (num_ == rhs.num_) && (den_ == rhs.den_);
}

bool Rational::operator!=(const Rational& rhs) const noexcept {
    return !operator==(rhs);
}

bool Rational::operator<(const Rational& rhs) const noexcept {
    return static_cast<std::int64_t>(num_) * rhs.den_ <
        static_cast<std::int64_t>(rhs.num_) * den_;
}

bool Rational::operator<=(const Rational& rhs) const noexcept {
    return static_cast<std::int64_t>(num_) * rhs.den_ <=
        static_cast<std::int64_t>(rhs.num_) * den_;
}

bool Rational::operator>(const Rational& rhs) const noexcept {
    return static_cast<std::int64_t>(num_) * rhs.den_ >
        static_cast<std::int64_t>(rhs.num_) * den_;
}

bool Rational::operator>=(const Rational& rhs) const noexcept {
    return static_cast<std::int64_t>(num_) * rhs.den_ >=
        static_cast<std::int64_t>(rhs.num_) * den_;
}

Rational& Rational::operator+=(const Rational& rhs) noexcept {
    num_ = static_cast<std::int32_t>(
        static_cast<std::int64_t>(num_) * rhs.den_ +
        static_cast<std::int64_t>(rhs.num_) * den_
        );
    den_ = static_cast<std::int32_t>(
        static_cast<std::int64_t>(den_) * rhs.den_
        );
    Normalize();
    return *this;
}

Rational& Rational::operator-=(const Rational& rhs) noexcept {
    num_ = static_cast<std::int32_t>(
        static_cast<std::int64_t>(num_) * rhs.den_ -
        static_cast<std::int64_t>(rhs.num_) * den_
        );
    den_ = static_cast<std::int32_t>(
        static_cast<std::int64_t>(den_) * rhs.den_
        );
    Normalize();
    return *this;
}

Rational& Rational::operator*=(const Rational& rhs) noexcept {
    num_ = static_cast<std::int32_t>(
        static_cast<std::int64_t>(num_) * rhs.num_
        );
    den_ = static_cast<std::int32_t>(
        static_cast<std::int64_t>(den_) * rhs.den_
        );
    Normalize();
    return *this;
}

Rational& Rational::operator/=(const Rational& rhs) {
    if (rhs.num_ == 0) {
        throw std::invalid_argument("Division by zero");
    }

    num_ = static_cast<std::int32_t>(
        static_cast<std::int64_t>(num_) * rhs.den_
        );
    den_ = static_cast<std::int32_t>(
        static_cast<std::int64_t>(den_) * rhs.num_
        );
    Normalize();
    return *this;
}

Rational& Rational::operator+=(const std::int32_t rhs) noexcept {
    return (*this += Rational(rhs));
}

Rational& Rational::operator-=(const std::int32_t rhs) noexcept {
    return (*this -= Rational(rhs));
}

Rational& Rational::operator*=(const std::int32_t rhs) noexcept {
    return (*this *= Rational(rhs));
}

Rational& Rational::operator/=(const std::int32_t rhs) {
    return (*this /= Rational(rhs));
}

std::istream& Rational::ReadFrom(std::istream& istrm) {
    
    std::istream::sentry sentry(istrm);
    if (!sentry) {
        return istrm;                 
    }

    std::string token;
    if (!(istrm >> token)) {           
        return istrm;               
    }


    std::size_t pos = 0;

    auto parse_signed_int = [&](std::int32_t& out) -> bool {
        if (pos >= token.size()) return false;

        int sign = 1;
        if (token[pos] == '+') {
            ++pos;
        }
        else if (token[pos] == '-') {
            sign = -1;
            ++pos;
        }

        if (pos >= token.size() || !std::isdigit(static_cast<unsigned char>(token[pos]))) {
            return false;             
        }

        long long value = 0;
        while (pos < token.size() && std::isdigit(static_cast<unsigned char>(token[pos]))) {
            value = value * 10 + (token[pos] - '0');
            if (value > std::numeric_limits<std::int32_t>::max()) {
                return false;       
            }
            ++pos;
        }
        out = static_cast<std::int32_t>(sign * value);
        return true;
        };

    std::int32_t num0 = 0;
    std::int32_t den0 = 0;

    // 1) ���������
    if (!parse_signed_int(num0)) {
        istrm.setstate(std::ios_base::failbit);
        return istrm;
    }

  
    if (pos >= token.size() || token[pos] != separator) {
        istrm.setstate(std::ios_base::failbit);
        return istrm;
    }
    ++pos;

   
    if (!parse_signed_int(den0)) {
        istrm.setstate(std::ios_base::failbit);
        return istrm;
    }


    if (pos != token.size()) {
   
        istrm.setstate(std::ios_base::failbit);
        return istrm;
    }

    if (den0 == 0) {
        throw std::invalid_argument("Zero denominator in Rational::ReadFrom");
    }

    num_ = num0;
    den_ = den0;
    Normalize();
    return istrm;
}



std::ostream& Rational::WriteTo(std::ostream& ostrm) const noexcept {
    ostrm << num_ << separator << den_ ;
    return ostrm;
}




Rational operator+(const Rational& lhs, const Rational& rhs) noexcept {
    Rational res(lhs);
    res += rhs;
    return res;
}

Rational operator-(const Rational& lhs, const Rational& rhs) noexcept {
    Rational res(lhs);
    res -= rhs;
    return res;
}

Rational operator*(const Rational& lhs, const Rational& rhs) noexcept {
    Rational res(lhs);
    res *= rhs;
    return res;
}

Rational operator/(const Rational& lhs, const Rational& rhs) {
    Rational res(lhs);
    res /= rhs;
    return res;
}

Rational operator+(const Rational& lhs, const std::int32_t rhs) noexcept {
    Rational res(lhs);
    res += rhs;
    return res;
}

Rational operator-(const Rational& lhs, const std::int32_t rhs) noexcept {
    Rational res(lhs);
    res -= rhs;
    return res;
}

Rational operator*(const Rational& lhs, const std::int32_t rhs) noexcept {
    Rational res(lhs);
    res *= rhs;
    return res;
}

Rational operator/(const Rational& lhs, const std::int32_t rhs) {
    Rational res(lhs);
    res /= rhs;
    return res;
}

Rational operator+(const std::int32_t lhs, const Rational& rhs) noexcept {
    Rational res(lhs);
    res += rhs;
    return res;
}

Rational operator-(const std::int32_t lhs, const Rational& rhs) noexcept {
    Rational res(lhs);
    res -= rhs;
    return res;
}

Rational operator*(const std::int32_t lhs, const Rational& rhs) noexcept {
    Rational res(rhs);
    res *= lhs;
    return res;
}

Rational operator/(const std::int32_t lhs, const Rational& rhs) {
    Rational res(lhs);
    res /= rhs;
    return res;
}

std::ostream& operator<<(std::ostream& ostrm, const Rational& rhs) noexcept {
    return rhs.WriteTo(ostrm);
}

std::istream& operator>>(std::istream& istrm, Rational& rhs) {
    return rhs.ReadFrom(istrm);
}
