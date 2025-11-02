#include <iostream>
#include <stdexcept>
#include <numeric>
class Rational {
        int m;
        int n;
        void Irreducible(int& a, int& b) {
                if (b < 0) {
                        a = -a;
                        b = -b;
                }
                int t = std::gcd(a, b);
                a = a / t;
                b = b / t;
        }
        public:
        Rational(int x = 0, int y = 1) {
                if (y == 0) {
                        throw std::invalid_argument("Деление на 0!");
                }
                Irreducible(x, y);
                m = x;
                n = y;
        }
        double to_double() const {
                return (double)m/n;
        }
        Rational* operator+=(const Rational other) {
                m = n * other.m + m * other.n;
                n = n * other.n;
                Irreducible(m, n);
                return this;
        }
        Rational* operator-=(const Rational other) {
                m = - n * other.m + m * other.n;
                n = n * other.n;
                Irreducible(m, n);
                return this;
        }
        Rational* operator*=(const Rational other) {
                m = m * other.m;
                n = n * other.n;
                Irreducible(m, n);
                return this;
        }
        Rational* operator/=(const Rational other) {
                m = m * other.n;
                n = n * other.m;
                Irreducible(m, n);
                return this;
        }
        Rational* operator++() {
                m = m + n;
                return this;
        }
        Rational* operator--() {
                m = m - n;
                return this;
        }
        Rational operator+(const Rational other) const {
                Rational sum = Rational(n * other.m + m * other.n, n * other.n);
                return sum;
        }
        Rational operator-(const Rational other) const {
                Rational dif = Rational(-n * other.m + m * other.n, n * other.n);
                return dif;
        }
        Rational operator*(const Rational other) const {
                Rational mul = Rational(m * other.m, n * other.n);
                return mul;
        }
        Rational operator/(const Rational other) const {
                Rational div = Rational(m * other.n, n * other.m);
                return div;
        }
        bool operator==(const Rational other) const {
                if ((m == other.m) and (n == other.n)) {
                        return true;
                }
                return false;
        }
        bool operator!=(const Rational other) const {
                if ((m == other.m) and (n == other.n)) {
                        return false;
                }
                return true;
        }
        bool operator>=(const Rational other) const {
                if ((m*other.n - n*other.m)/(n*other.n) >= 0) {
                        return true;
                }
                return false;
        }
        bool operator<=(const Rational other) const {
                if ((m*other.n - n*other.m)/(n*other.n) <= 0) {
                        return true;
                }
                return false;
        }
        bool operator>(const Rational other) const {
                if ((m*other.n - n*other.m)/(n*other.n) > 0) {
                        return true;
                }
                return false;
        }
        bool operator<(const Rational other) const {
                if ((m*other.n - n*other.m)/(n*other.n) < 0) {
                        return true;
                }
                return false;
        }
        void print() const {
                std::cout << m << '/' << n << '\n';
        }
};
int main() {
        Rational a(1, 2);
        Rational b(2, 3);
        if (a > b) {
                a.print();
        } else {
                b.print();
        }
        a+=b;
        a.print();
        Rational c = a*b;
        c.print();
        return 0;
}
