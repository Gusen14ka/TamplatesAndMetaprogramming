#include <tuple>
#include <array>
#include <iostream>
#include "hpp/templatePrint.hpp"

int main() {
    std::tuple<int,std::string,double> t{42, "hello", 3.14};
    std::cout << t << "\n";                // prints: (42, hello, 3.14)

    std::pair<int,int> p{1,2};
    std::cout << p << "\n";                // prints: (1, 2)

    std::array<char,4> a{'a','b','c','d'};
    std::cout << a << "\n";                // prints: (a, b, c, d)
}