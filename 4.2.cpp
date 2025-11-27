#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
class BCD {
private:
  std::vector<uint8_t> num = {};
  int prec = 0;
  int sign = 1;
public:
  BCD() : num{}, prec(0), sign(1) {}
  BCD(const std::string& s, int k = -1) {
    if (s.empty()) {
      num.push_back(0);
      return;
    }
    int len_int = 0;
    int i = 0;
    if (s[i] == '-') {
      sign = -1;
      i++;
    } else {
      sign = 1;
    }
    while ((i < s.size()) and (s[i] != '.')) {
      assert(s[i] >= '0' && s[i] <= '9' && "Недопустимый символ!");
      len_int++;
      num.push_back(s[i] - '0');
      i++;
    }
    assert(len_int <= 10 && "В целой части может быть не более 10 цифр!");
    if (k != -1) {
      if (i == s.size()) {
	for (int j = 0; j < k; j++) {
	  num.push_back(0);
	}
	prec = k;
      } else {
	i++;
	while (prec < k) {
	  if (i >= s.size()) {
	    num.push_back(0);
	  } else {
	    assert(s[i] >= '0' && s[i] <= '9' && "Недопустимый символ!");
	    num.push_back(s[i] - '0');
	  }
	  i++;
	  prec++;
	}
      }
    } else {
      if (i < s.size()) {
        i++;
        while (i < s.size()) {
          assert(s[i] >= '0' && s[i] <= '9' && "Недопустимый символ!");
          num.push_back(s[i] - '0');
	      prec++;
	      i++;
        }
      }
    }
  }
  BCD(const BCD& other) {
    num = other.num;
    prec = other.prec;
    sign = other.sign;
  }
  BCD(BCD&& other) noexcept {
    num = std::move(other.num);
    prec = other.prec;
    sign = other.sign;
    other.prec = 0;
    other.sign = 1;
  }
  BCD& operator=(const BCD& other) {
    if (this != &other) {
      num = other.num;
      prec = other.prec;
      sign = other.sign;
    }
    return *this;
  }
  BCD& operator=(BCD&& other) noexcept {
    if (this != &other) {
      num = std::move(other.num);
      prec = other.prec;
      sign = other.sign;
      other.prec = 0;
      other.sign = 1;
    }
    return *this;
  }
  BCD operator+() const {
    return *this;
  }
  BCD operator-() const {
    BCD opp = *this;
    opp.sign *= -1;
    return opp;
  }
  BCD operator+(const BCD& other) const {
    int k = std::max(prec, other.prec);
    int shift = 0;
    BCD c1 = *this;
    BCD c2 = other;
    BCD sum;
    if (c1.sign > c2.sign) {
      return c1 - (-c2);
    }
    if (c1.sign < c2.sign) {
      return c2 - (-c1);
    }
    sum.sign = c1.sign;
    while (c1.prec != k) {
      c1.num.push_back(0);
      c1.prec++;
    }
    while (c2.prec != k) {
      c2.num.push_back(0);
      c2.prec++;
    }
    for (int i = k - 1; i >= 0; i--) {
      uint8_t dig = shift + c1.num[c1.num.size() - k + i] + c2.num[c2.num.size() - k + i];
      sum.num.push_back(dig % 10);
      shift = dig / 10;
    }
    int len_int = 0;
    int l1 = c1.num.size() - c1.prec;
    int l2 = c2.num.size() - c2.prec;
    if (l1 >= l2) {
      for (int i = l2 - 1; i >= 0; i--) {
        uint8_t dig = shift + c1.num[l1 - l2 + i] + c2.num[i];
	    sum.num.push_back(dig % 10);
	    shift = dig / 10;
	    len_int++;
      }
      for (int i = l1 - l2 - 1; i >= 0; i--) {
        uint8_t dig = shift + c1.num[i];
	    sum.num.push_back(dig % 10);
        shift = dig / 10;
	    len_int++;
      }
      while (shift > 0) {
        sum.num.push_back(shift % 10);
	    shift = shift / 10;
	    len_int++;
      }
      assert(len_int <= 10 && "Выход за допустимые границы!");
    } else {
      for (int i = l1 - 1; i >= 0; i--) {
          uint8_t dig = shift + c1.num[i] + c2.num[l2 - l1 + i];
          sum.num.push_back(dig % 10);
          shift = dig / 10;
          len_int++;
      }
      for (int i = l2 - l1 - 1; i >= 0; i--) {
        uint8_t dig = shift + c2.num[i];
        sum.num.push_back(dig % 10);
        shift = dig / 10;
        len_int++;
      }
      while (shift > 0) {
        sum.num.push_back(shift % 10);
        shift = shift / 10;
        len_int++;
      }
      assert(len_int <= 10 && "Выход за допустимые границы!");
    }
    sum.prec = std::min(prec, other.prec);
    if (sum.prec != 0) {
      sum.prec--;
    }
    std::reverse(sum.num.begin(), sum.num.end());
    while (k != sum.prec) {
      sum.num.pop_back();
      k--;
    }
    return sum;
  }
  BCD operator-(const BCD& other) const {
    int k = std::max(prec, other.prec);
    BCD c1 = *this;
	BCD c2 = other;
    BCD dif;
    if (c1.sign > c2.sign) {
      return c1 + (-c2);
    }
    if (c2.sign > c1.sign) {
      return -(c2 + (-c1));
    }
    if (c1.sign == -1) {
      c1 = -c1;
      c2 = -c2;
      if (c2 < c1) {
        dif.sign = -1;
      } else {
        std::swap(c1, c2);
      }
    } else {
      if (c1 < c2) {
        dif.sign = -1;
	    std::swap(c1, c2);
      }
    }
    while (c1.prec != k) {
      c1.num.push_back(0);
      c1.prec++;      
    }
    while (c2.prec != k) {
      c2.num.push_back(0);
      c2.prec++;
    }
    int shift = 0;
    for (int i = c2.num.size() - 1; i >= 0; i--) {
      int j = c1.num.size() - c2.num.size() + i;
      int delta = c1.num[j] - c2.num[i] - shift;
      if (delta >= 0) {
        dif.num.push_back((uint8_t)delta);
	    shift = 0;
      } else {
        dif.num.push_back((uint8_t)(10 + delta));
	    shift = 1;
      }
    }
    for (int i = c1.num.size() - c2.num.size() - 1; i >= 0; i--) {
      uint8_t delta = c1.num[i] - shift;
      if (delta >= 0) {
        dif.num.push_back(delta);
	    shift = 0;
      } else {
        dif.num.push_back(10 + delta);
	    shift = 1;
      }
    }
    int i = dif.num.size() - 1;
    while (true) {
      if ((dif.num[i] == 0) and (i > k)) {
        dif.num.pop_back();
	    i--;
      } else {
        break;
      }
    }
    dif.prec = std::min(prec, other.prec);
    if (dif.prec != 0) {
      dif.prec--;
    }
    std::reverse(dif.num.begin(), dif.num.end());
    while (k != dif.prec) {
      dif.num.pop_back();
      k--;
    }
    return dif;
  }
  BCD operator*(const BCD& other) const {
    BCD comp;
    BCD m1 = *this;
    BCD m2 = other;
    comp.sign = m1.sign * m2.sign;
    int k = prec_of_comp(m1, m2);
    comp.prec = k;
    std::vector<int> res(m1.num.size() + m2.num.size(), 0);
    for (int i = m1.num.size() - 1; i >= 0; i--) {
      for (int j = m2.num.size() - 1; j >= 0; j--) {
        res[i + j + 1] += m1.num[i] * m2.num[j];
      }
    }
    for (int i = res.size() - 1; i > 0; i--) {
      res[i - 1] += res[i] / 10;
      res[i] %= 10;
    }
    int start = 0;
    while (true) {
      if ((start < res.size()) and (res[start] == 0)) {
        start++;
      } else {
        break;
      }
    }
    int x = res.size() - start - m1.prec - m2.prec;
    if (x > 0) {
      for (int i = 0; i < x + k; i++) {
        comp.num.push_back(res[i + start]);
      }
    } else {
      x = 1 - x;
      for (int i = 0; i < x; i++) {
        comp.num.push_back(0);
      }
      for (int i = 0; i < k - x + 1; i++) {
        comp.num.push_back(res[start + i]);
      }
    }
    return comp;
  }
  bool operator==(const BCD& other) const {
    if ((num.size() != other.num.size()) or (prec != other.prec) or (sign != other.sign)) {
      return false;
    }
    for (int i = 0; i < num.size(); i++) {
      if (num[i] != other.num[i]) {
        return false;
      }
    }
    return true;
  }
  bool operator!=(const BCD& other) const {
    return !(*this == other);
  }
  bool operator>(const BCD& other) const {
    if (sign != other.sign) {
      return sign > other.sign;
    }
    if (sign == -1) {
      return (-*this) < (-other);
    }
    if (num.size() - prec != other.num.size() - other.prec) {
      return num.size() - prec > other.num.size() - other.prec;
    }
    for (int i = 0; i < std::min(num.size(), other.num.size()); i++) {
      if (num[i] != other.num[i]) {
        return num[i] > other.num[i];
      }
    }
    if (num.size() > other.num.size()) {
      for (int i = other.num.size(); i < num.size(); i++) {
        if (num[i] != 0) {
	  return true;
	}
      }
    }
    return false;
  }
  bool operator<(const BCD& other) const {
    if (sign != other.sign) {
      return sign < other.sign;
    }
    if (sign == -1) {
      return (-*this) > (-other);
    }
    if (num.size() - prec != other.num.size() - other.prec) {
      return num.size() - prec < other.num.size() - other.prec;
    }
    for (int i = 0; i < std::min(num.size(), other.num.size()); i++) {
      if (num[i] != other.num[i]) {
        return num[i] < other.num[i];
      }
    }
    if (num.size() < other.num.size()) {
      for (int i = num.size(); i < other.num.size(); i++) {
        if (other.num[i] != 0) {
          return true;
        }
      }
    }
    return false;
  }
  bool operator>=(const BCD& other) const {
    if (*this == other) {
      return true;
    }
    return *this > other;
  }
  bool operator<=(const BCD& other) const {
    if (*this == other) {
      return true;
    }
    return *this < other;
  }
  int prec_of_comp(const BCD& a, const BCD& b) const {
    int ceil_a = 0;
    int ceil_b = 0;
    for (int i = a.num.size() - 1; i >= a.num.size() - a.prec; i--) {
      if (a.num[i] != 0) {
        ceil_a++;
	    break;
      }
    }
    for (int i = b.num.size() - 1; i >= b.num.size() - b.prec; i--) {
      if (b.num[i] != 0) {
        ceil_b++;
        break;
      }
    }
    for (int i = a.num.size() - a.prec - 1; i >= 0; i--) {
      ceil_a += a.num[i] * std::pow(10, a.num.size() - a.prec - 1 - i);
    }
    for (int i = b.num.size() - b.prec - 1; i >= 0; i--) {
      ceil_b += b.num[i] * std::pow(10, b.num.size() - b.prec - 1 - i);
    }
    double res = std::min(a.prec, b.prec) - 1 - log10(std::max(ceil_a, ceil_b));
    return std::max(0, static_cast<int>(res));
  }
  static BCD div_n(int n, int k) {
    BCD res;
    res.prec = k;
    res.sign = 1;
    res.num.push_back(0);
    int r = 1;
    int i = 0;
    while (i < k) {
      res.num.push_back((r * 10) / n);
      r = (r * 10) % n;
      i++;
    }
    return res;
  }
  static BCD exp_100() {
    int n = 70;
    int k = 300;
    BCD exp = BCD("0", k);
    BCD m = BCD("1", k);
    exp = exp + m;
    for (int i = 1; i <= n; i++) {
      BCD mm = BCD::div_n(i, k);
      m = m * mm;
      exp = exp + m;
    }
    std::cout << (int)exp.num[0] << ',';
    for (int i = 1; i <= 100; i++) {
      std::cout << (int)exp.num[i]; 
    }
    std::cout << '\n';
    return exp;
  }
  void print() {
    if (sign == -1) {  
      std::cout << '-';
    }
    for (int i = 0; i < num.size() - prec; i++) {
      std::cout << (int)num[i];
    }
    if (prec != 0) {
      std::cout << ',';
    }
    for (int i = num.size() - prec; i < num.size(); i++) {
      std::cout << (int)num[i];
    }
    std::cout << '\n';
  }
};
int main() {
  BCD::exp_100();
  BCD a = BCD("12.345", 4);
  BCD b = BCD("-5.4321", 3);
  a.print();
  b.print();
  (a + b).print();
  (a - b).print();
  (a * b).print();
}
