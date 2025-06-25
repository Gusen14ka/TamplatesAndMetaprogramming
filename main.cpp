#include <iostream>
#include "hpp/templateFactorial.hpp"

int main(){
    try {
        constexpr int n = factorial<1>();
    } catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }
}