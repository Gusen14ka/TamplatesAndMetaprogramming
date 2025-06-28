#include <cassert>
#include <type_traits>
#include <iostream>

// Подключаем ваш заголовок с add(...)
#include "hpp/templateAdd.hpp"

int main() {
    // 1) int + unsigned int → оба 4 байта, смешанный signed/unsigned → приоритет signed → int
    {
        using R = decltype(add(  1,   2u));
        static_assert(std::is_same_v<R, int>);
        constexpr R r = add(1, 2u);
        assert(r == 3);
    }

    // 2) unsigned int + int (обратный порядок аргументов) → тот же результат
    {
        using R = decltype(add( 10u,  5));
        static_assert(std::is_same_v<R, int>);
        constexpr R r = add(10u, 5);
        assert(r == 15);
    }

    // 3) int + long long → common_type = long long (оба signed)
    {
        using R = decltype(add(  1,   2ll));
        static_assert(std::is_same_v<R, long long>);
        constexpr R r = add(1, 2ll);
        assert(r == 3ll);
    }

    // 4) unsigned long + unsigned short → оба unsigned, common_type = unsigned long
    {
        unsigned long ul = 100ul;
        unsigned short us = 23;
        using R = decltype(add(ul, us));
        static_assert(std::is_same_v<R, unsigned long>);
        R r = add(ul, us);
        assert(r == 123ul);
    }

    // 5) float + double → common_type = double (оба floating-point)
    {
        using R = decltype(add(1.5f, 2.25));
        static_assert(std::is_same_v<R, double>);
        R r = add(1.5f, 2.25);
        assert(r == 3.75);
    }

    // 6) short + unsigned short (оба 2 байта, mixed) → signed priority → short
    {
        using R = decltype(add(static_cast<short>(-5), static_cast<unsigned short>(8)));
        static_assert(std::is_same_v<R, short>);
        R r = add(static_cast<short>(-5), static_cast<unsigned short>(8));
        assert(r == 3);
    }

    // 7) long + unsigned long long (sizes differ: 8 vs 8 on many platforms, mixed) 
    //    sizes equal on LP64 → priority signed → make_signed<common_type> = long long
    {
        using R = decltype(add(  7l,  2ull));
        static_assert(std::is_same_v<R, long long>);
        R r = add(7l, 2ull);
        assert(r == 9ll);
    }

    {
        char a = 'A';
        char b = 'B';
        using R = decltype(add(a, b));
        static_assert(std::is_same_v<R, char>,
                      "char + char -> char");
    }

    {
        std::string a = "Hello, ";
        std::string b = "World!";
        using R = decltype(add(a, b));
        static_assert(std::is_same_v<R, std::string>,
                      "std::string + std::string -> std::string");
        R r = add(a, b);
        assert(r == "Hello, World!");
    }

    // 2) const char* + std::string
    {
        const char* a = "Foo";
        std::string b = "Bar";
        using R = decltype(add(a, b));
        static_assert(std::is_same_v<R, std::string>,
                      "const char* + std::string -> std::string");
        R r = add(a, b);
        assert(r == "FooBar");
    }

    // 3) std::string + const char*
    {
        std::string a = "X";
        const char* b = "Y";
        using R = decltype(add(a, b));
        static_assert(std::is_same_v<R, std::string>,
                      "std::string + const char* -> std::string");
        R r = add(a, b);
        assert(r == "XY");
    }

    // 4) const char* + const char*
    {
        const char* a = "123";
        const char* b = "456";
        using R = decltype(add(a, b));
        static_assert(std::is_same_v<R, std::string>,
                      "const char* + const char* -> std::string");
        R r = add(a, b);
        assert(r == "123456");
    }

    std::cout << "All tests passed!\n";
    return 0;
}
