#include "../hpp/templateTypeList.hpp"
#include <type_traits>
#include <iostream>

// 0) TypeList
using Empty = TypeList<>;
using TL1 = TypeList<int, double, char>;
using TL2 = TypeList<float, int, double, char>;
using TL3 = TypeList<int, int, double, int>;

// 1) Length
static_assert(Length<Empty>::value == 0, "Length<Empty> failed");
static_assert(Length<TL1>::value == 3, "Length<TL1> failed");
static_assert(Length<TL2>::value == 4, "Length<TL2> failed");

// 2) IndexOf
static_assert(IndexOf<TL1, int>::value == 0, "IndexOf<int, TL1> failed");
static_assert(IndexOf<TL1, double>::value == 1, "IndexOf<double, TL1> failed");
static_assert(IndexOf<TL1, char>::value == 2, "IndexOf<char, TL1> failed");
static_assert(IndexOf<TL1, float>::value == -1, "IndexOf<float, TL1> failed");
static_assert(IndexOf<Empty, int>::value == -1, "IndexOf<int, Empty> failed");

// 3) PushFront
using TL1WithBool = PushFront<TL1, bool>::type;
static_assert(std::is_same_v<TL1WithBool, TypeList<bool, int, double, char>>, "PushFront<bool, TL1> failed");
using TL2WithChar = PushFront<TL2, char>::type;
static_assert(std::is_same_v<TL2WithChar, TypeList<char, float, int, double, char>>, "PushFront<char, TL2> failed");

// 4) Erase
using TL3ErasedInt = Erase<TL3, int>::type;
static_assert(std::is_same_v<TL3ErasedInt, TypeList<double>>, "Erase<int, TL3> failed");
using TL1ErasedDouble = Erase<TL1, double>::type;
static_assert(std::is_same_v<TL1ErasedDouble, TypeList<int, char>>, "Erase<double, TL1> failed");
using TL1ErasedChar = Erase<TL1, char>::type;
static_assert(std::is_same_v<TL1ErasedChar, TypeList<int, double>>, "Erase<char, TL1> failed");
using TL1ErasedFloat = Erase<TL1, float>::type;
static_assert(std::is_same_v<TL1ErasedFloat, TL1>, "Erase<float, TL1> failed");
using EmptyErasedInt = Erase<Empty, int>::type;
static_assert(std::is_same_v<EmptyErasedInt, Empty>, "Erase<int, Empty> failed");

// 1) Length (алиас)
static_assert(Length_t<Empty> == 0, "Length_t<Empty> failed");
static_assert(Length_t<TL1> == 3, "Length_t<TL1> failed");
static_assert(Length_t<TL2> == 4, "Length_t<TL2> failed");

// 2) IndexOf (алиас)
static_assert(IndexOf_t<TL1, int> == 0, "IndexOf_t<int, TL1> failed");
static_assert(IndexOf_t<TL1, double> == 1, "IndexOf_t<double, TL1> failed");
static_assert(IndexOf_t<TL1, char> == 2, "IndexOf_t<char, TL1> failed");
static_assert(IndexOf_t<TL1, float> == -1, "IndexOf_t<float, TL1> failed");
static_assert(IndexOf_t<Empty, int> == -1, "IndexOf_t<int, Empty> failed");

// 3) PushFront (алиас)
using TL1WithBool = PushFront_t<TL1, bool>;
static_assert(std::is_same_v<TL1WithBool, TypeList<bool, int, double, char>>, "PushFront_t<bool, TL1> failed");
using TL2WithChar = PushFront_t<TL2, char>;
static_assert(std::is_same_v<TL2WithChar, TypeList<char, float, int, double, char>>, "PushFront_t<char, TL2> failed");

// 4) Erase (алиас)
using TL3ErasedInt = Erase_t<TL3, int>;
static_assert(std::is_same_v<TL3ErasedInt, TypeList<double>>, "Erase_t<int, TL3> failed");
using TL1ErasedDouble = Erase_t<TL1, double>;
static_assert(std::is_same_v<TL1ErasedDouble, TypeList<int, char>>, "Erase_t<double, TL1> failed");
using TL1ErasedChar = Erase_t<TL1, char>;
static_assert(std::is_same_v<TL1ErasedChar, TypeList<int, double>>, "Erase_t<char, TL1> failed");
using TL1ErasedFloat = Erase_t<TL1, float>;
static_assert(std::is_same_v<TL1ErasedFloat, TL1>, "Erase_t<float, TL1> failed");
using EmptyErasedInt = Erase_t<Empty, int>;
static_assert(std::is_same_v<EmptyErasedInt, Empty>, "Erase_t<int, Empty> failed");

// 5) Проверка вывода (необязательно, просто для примера)
int main() {
    std::cout << "Length<TL1>: " << Length<TL1>::value << std::endl;
    std::cout << "IndexOf<double, TL1>: " << IndexOf<TL1, double>::value << std::endl;
    std::cout << "Length<Erase<TL3, int>>: " << Length<TL3ErasedInt>::value << std::endl;
    return 0;
}