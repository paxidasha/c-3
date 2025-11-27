#include <algorithm>
#include <cmath>
#include <cctype>
#include <functional>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <stdexcept>
class Operation {
public:
  virtual void run(std::stack<double>& stack) = 0;
  virtual ~Operation() {}
};
class UnaryOperation : public Operation {
private:
  std::function<double(double)> func;
public:
  UnaryOperation(std::function<double(double)> f) : func(f) {}
  void run(std::stack<double>& stack) override {
    if (stack.size() < 1) {
      throw std::runtime_error("Недостаточно переменных для выполнения операции!");
    }
    double x = stack.top();
    stack.pop();
    stack.push(func(x));
  }
};
class BinaryOperation : public Operation {
private:
  std::function<double(double, double)> func;
public:
  BinaryOperation(std::function<double(double, double)> f) : func(f) {}
  void run(std::stack<double>& stack) override {
    if (stack.size() < 2) {
      throw std::runtime_error("Недостаточно переменных для выполнения операции!");
    }
    double x = stack.top();
    stack.pop();
    double y = stack.top();
    stack.pop();
    stack.push(func(y, x));
  }
};
class TernarOperation : public Operation {
private:
  std::function<double(double, double, double)> func;
public:
  TernarOperation(std::function<double(double, double, double)> f) : func(f) {}
  void run(std::stack<double>& stack) override {
    if (stack.size() < 3) {
      throw std::runtime_error("Недостаточно переменных для выполнения операции!");
    }
    double x = stack.top();
    stack.pop();
    double y = stack.top();
    stack.pop();
    double z = stack.top();
    stack.pop();
    stack.push(func(z, y, x));
  }
};
int main() {
  std::map<std::string, Operation*> operations;
  operations["sin"] = new UnaryOperation([](double x) { return std::sin(x); });
  operations["cos"] = new UnaryOperation([](double x) { return std::cos(x); });
  operations["tg"] = new UnaryOperation([](double x) { return std::tan(x); });
  operations["ctg"] = new UnaryOperation([](double x) { return 1 / std::tan(x); });
  operations["exp"] = new UnaryOperation([](double x) { return std::exp(x); });
  operations["log"] = new UnaryOperation([](double x) { return std::log(x); });
  operations["sqrt"] = new UnaryOperation([](double x){ return std::sqrt(x); });
  operations["+"] = new BinaryOperation([](double x, double y){ return x + y; });
  operations["-"] = new BinaryOperation([](double x, double y){ return x - y; });
  operations["*"] = new BinaryOperation([](double x, double y){ return x * y; });
  operations["/"] = new BinaryOperation([](double x, double y){ return x / y; });
  operations["atan2"] = new BinaryOperation([](double x, double y){ return std::atan2(x, y); });
  operations["pow"] = new BinaryOperation([](double x, double y){ return std::pow(x, y); });
  operations["median"] = new TernarOperation([](double x, double y, double z){ return x + y + z - std::min({x, y, z}) - std::max({x, y, z}); });
  std::stack<double> stack;
  std::string input;
  std::getline(std::cin, input);
  std::string s = "";
  for (int i = 0; i < (int)input.size(); i++) {
    if (input[i] != ' ') {
      s += input[i];
    } else {
      if ((std::isdigit(s[0]) != 0) or ((s[0] == '-') and (s.size() > 1))) {
        if ((std::count(s.begin(), s.end(), '.') >= 2) or ((s[0] == '-') and (s[1] == '.'))) {
          throw std::runtime_error("Неправильный формат введенного числа!");
        }
        for (int i = 1; i < (int)s.size(); i++) {
          if (((s[i] != '.') and (std::isdigit(s[i]) == 0)) or ((i == (int)s.size() - 1) and (s[i] == '.'))) {
            throw std::runtime_error("Неправильный формат введенного числа!");
          }
        }
        stack.push(std::stod(s));
      } else {
        if (operations.find(s) == operations.end()) {
	  throw std::runtime_error("Введенная функция отсутсвует!");
	}
	operations[s]->run(stack);
      }
      s = "";
    }
  }
  if ((std::isdigit(s[0]) != 0) or ((s[0] == '-') and (s.size() > 1))) {
    if ((std::count(s.begin(), s.end(), '.') >= 2) or ((s[0] == '-') and (s[1] == '.'))) {
      throw std::runtime_error("Неправильный формат введенного числа!");
    }
    for (int i = 1; i < (int)s.size(); i++) {
      if (((s[i] != '.') and (std::isdigit(s[i]) == 0)) or ((i == (int)s.size() - 1) and (s[i] == '.'))) {
        throw std::runtime_error("Неправильный формат введенного числа!");
      }
    }
    stack.push(std::stod(s));
  } else {
    if (operations.find(s) == operations.end()) {
      throw std::runtime_error("Введенная функция отсутсвует!");
    }
    operations[s]->run(stack);
  }
  if (stack.size() != 1) {
    throw std::runtime_error("Данные были введены некорректно!");
  }
  std::cout << stack.top() << '\n';
  return 0;
}
