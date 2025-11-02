#include <iostream>
#include <vector>
#include <stdexcept>
class IP {
       std::vector<int> ip;
       public:
       IP(std::vector<int> a = {0, 0, 0, 0}) {
               if (a.size() != 4) {
                      throw std::invalid_argument("IP должен содержать 4 элемента!");
               }
               for (int i = 0; i < 4; i++) {
                       if ((a[i] < 0) or (a[i] > 255)) {
                                throw std::invalid_argument("Каждый элемент IP должен быть числом от 0 до 255!");
                       }
                }
                ip = a;
        }
        void input() {
                int a1, a2, a3, a4;
                std::cin >> a1 >> a2 >> a3 >> a4;
                std::vector<int> a = {a1, a2, a3, a4};
                for (int i = 0; i < 4; i++) {
                        if ((a[i] < 0) or (a[i] > 255)) {
                                throw std::invalid_argument("Каждый элемент IP должен быть числом от 0 до 255!");
                        }
                }
                ip = a;
        }
        void print() const {
                std::cout << ip[0] << '.' << ip[1] << '.' << ip[2] << '.' << ip[3] << '\n';
        }
        std::vector<int> value() const {
                return ip;
        }
        void increase_ip() {
                for (int i = 3; i >= 0; i--) {
                        if (ip[i] < 255) {
                                ip[i]++;
                                break;
                        } else {
                                ip[i] = 0;
                        }
                }
        }
};
int main() {
        IP ip1, ip2;
        ip1.input();
        ip2.input();
        if (ip1.value() <= ip2.value()) {
                while (ip1.value() != ip2.value()) {
                        ip1.print();
                        ip1.increase_ip();
                }
                ip2.print();
        } else {
                while (ip1.value() != ip2.value()) {
                        ip2.print();
                        ip2.increase_ip();
                }
                ip1.print();
        }
        return 0;
}
