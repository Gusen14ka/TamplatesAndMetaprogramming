#pragma once
#include <concepts>
#include <type_traits>
#include <string>

// Concept to check if T is numeric integral
template <typename T>
concept NumericIntegral =
    std::is_integral_v<T> && !std::is_same_v<T, bool>;

// Concept to check if T is numeric arithmetic (numeric integral or floating point)
template <typename T>
concept MyArithmetic = 
    NumericIntegral<T> || std::floating_point<T>;

// Concept to check if T is a string-like type
template <typename T>
concept StringLike =
    std::same_as<std::remove_cvref_t<T>, std::string> ||
    std::same_as<std::remove_cvref_t<T>, std::string_view> ||
    std::same_as<std::remove_cvref_t<T>, const char*>;

// Scheme to deduce the type for addition of two NumericIntegral types
// The basic rule of type deduction is intentionally violated.
// The result of addition of two NumericIntegral always has the same weight as the bigger of the two types.
// If the types are mixed signed/unsigned, the result is signed.
template <NumericIntegral T, NumericIntegral U>
struct MyIntegralTypeDeduction{
    static constexpr bool is_mix_signed = 
        (std::is_signed_v<T> && std::is_unsigned_v<U>) ||
        (std::is_signed_v<U> && std::is_unsigned_v<T>);

    using commonType = std::common_type_t<T, U>;
    using biggerType = typename std::conditional<(sizeof(T) > sizeof(U)), T, U>::type;
    
    using type = std::conditional_t<is_mix_signed
    , std::conditional_t<(sizeof(commonType) > sizeof(U)) && (sizeof(commonType) > sizeof(T)), std::make_signed_t<biggerType>, std::make_signed_t<commonType>>
    , std::common_type_t<T, U>>;
};

template <MyArithmetic T, MyArithmetic U>
constexpr auto add(T a, U b){
    if constexpr (NumericIntegral<T> && NumericIntegral<U>) {
        return static_cast<MyIntegralTypeDeduction<T, U>::type>(a + b);
    }
    else 
    {
        return static_cast<std::common_type_t<T, U>>(a + b);
    }
}

template <StringLike T, StringLike U>
constexpr auto add(const T& a, const U& b) {
    std::string r{a};
    r += b; // Concatenate strings
    return r;
}


