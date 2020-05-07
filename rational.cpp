#include <iostream>
using namespace std;

int gcd(int a, int b) {
    while (b) {
	   a %= b;
	   swap(a, b);
	}
	return a;
}

int lcm(int a, int b) {
    return a / gcd(a, b) * b;
}

class Rational {
public:
    Rational() {
        num = 0;
        denom = 1;
    }

    Rational(int numerator, int denominator) {
        num = numerator;
        denom = denominator;
        if (num == 0) {
            denom = 1;
        } else {
            if (denom < 0) {
                denom = -denom;
                num = -num;
            }
            int tmp = gcd(abs(num), denom);
            num /= tmp;
            denom /= tmp;
        }
    }

    int Numerator() const {
        return num;
    }

    int Denominator() const {
        return denom;
    }

private:
    int num;
    int denom;
};

Rational operator+(const Rational& lhs, const Rational& rhs) {
    int new_denom = lcm(lhs.Denominator(), rhs.Denominator());
    int new_num_lhs = new_denom / lhs.Denominator() * lhs.Numerator();
    int new_num_rhs = new_denom / rhs.Denominator() * rhs.Numerator();
    return Rational(new_num_lhs + new_num_rhs, new_denom);
}

Rational operator-(const Rational& lhs, const Rational& rhs) {
    int new_denom = lcm(lhs.Denominator(), rhs.Denominator());
    int new_num_lhs = new_denom / lhs.Denominator() * lhs.Numerator();
    int new_num_rhs = new_denom / rhs.Denominator() * rhs.Numerator();
    return Rational(new_num_lhs - new_num_rhs, new_denom);
}

bool operator==(const Rational& lhs, const Rational& rhs) {
    return lhs.Numerator() == rhs.Numerator() && lhs.Denominator() == rhs.Denominator();
}

Rational operator*(const Rational& lhs, const Rational& rhs) {
    return Rational(lhs.Numerator() * rhs.Numerator(), lhs.Denominator() * rhs.Denominator());
}

Rational operator/(const Rational& lhs, const Rational& rhs) {
    return Rational(lhs.Numerator() * rhs.Denominator(), lhs.Denominator() * rhs.Numerator());
}

istream& operator>>(istream& stream, Rational& rhs) {
    int num, denom;
    bool full_read = true;
    full_read &= bool(stream >> num);
    stream.ignore(1);
    full_read &= bool(stream >> denom);
    if (full_read) {
        rhs = Rational(num, denom);
    }
    return stream;
}

ostream& operator<<(ostream& stream, const Rational rhs) {
    stream << rhs.Numerator() << '/' << rhs.Denominator();
    return stream;
}


bool operator<(const Rational& lhs, const Rational& rhs) {
    int new_denom = lcm(lhs.Denominator(), rhs.Denominator());
    int new_num_lhs = new_denom / lhs.Denominator() * lhs.Numerator();
    int new_num_rhs = new_denom / rhs.Denominator() * rhs.Numerator();
    return new_num_lhs < new_num_rhs;
}


bool operator>(const Rational& lhs, const Rational& rhs) {
    return rhs < lhs;
}

int main() {}
