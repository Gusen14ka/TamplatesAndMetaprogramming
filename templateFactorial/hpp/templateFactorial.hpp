#pragma once
#include <cmath>
#include <concepts>
#include <stdexcept>

// Concept to check if T is numeric integral
template <typename T>
concept NumericIntegral =
    std::same_as<int, T> || std::same_as<long, T> ||
    std::same_as<long long, T> || std::same_as<unsigned int, T> ||
    std::same_as<unsigned long, T> || std::same_as<unsigned long long, T>
    || std::same_as<short, T> || std::same_as<unsigned short, T>;


// No-type template function for factorial
// This function computes the factorial of a non-negative numeric integer at compile time.
template<auto val>
constexpr auto factorial(){
    static_assert(NumericIntegral<decltype(val)>, "Value must be an integral numeric type.");
    static_assert(val >= 0, "Factorial is not defined for negative numbers.");
    return val * factorial<val - 1>();
}

// Specializations for base cases n = 0
template<>
constexpr auto factorial<0>() {
    return 1;
}

// Specializations for base cases n = 1
template<>
constexpr auto factorial<1>(){
    return 1;
}

// Overloaded factorial function for constexpr runtime evaluation for numeric integral types
// Compiler will choose complile-time or runtime based on the constexpr context
template<NumericIntegral T>
constexpr T factorial (T val){
    if (val < static_cast<T>(0)){
        throw std::invalid_argument("Val must be positive");
    }
    else if (val == 0 || val == 1){
        return 1;
    }
    else{
        return val * factorial(val - 1);
    }
}

// Overloaded factorial function for floating-point types at runtime
// This uses the gamma function to compute the factorial for non-integer values.
template<std::floating_point T>
T factorial(T val){
    if (val < static_cast<T>(0)){
        throw std::invalid_argument("Val must be positive");
    }
    else{
        return std::tgamma(val + 1);
    }
}