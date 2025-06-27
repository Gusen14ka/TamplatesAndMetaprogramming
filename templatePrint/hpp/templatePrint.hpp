#pragma once
#include "hpp/templatePrint.hpp"
#include <cstddef>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>

// Concept for checking if a type is a tuple-like type
template <typename T>
concept TupleLikeC = requires {
    typename std::tuple_size<std::remove_cvref_t<T>>::type;
    {std::get<0>(std::declval<T>())};
};

// Function to print a tuple to an output stream
// Uses fold expressions to iterate over the tuple elements
template <TupleLikeC TupleType, std::size_t ...Ind>
inline void printTupleToStream(std::ostream& os, const TupleType& tp
    , std::index_sequence<Ind...>)
{
    std::size_t index = 0;
    
    auto printElem = [&index, &os] (const auto& x){
        if (index++ > 0){
            os << ", ";
        }
        os << x;
    };

    os << "(";
    (printElem(std::get<Ind>(tp)), ...);
    os << ")";
}

// Overloaded operator<< for printing tuples
// Kind of alias for printTupleToStream
template <TupleLikeC TupleType>
std::ostream& operator << (std::ostream& os, const TupleType& tp){
    printTupleToStream(os, tp, std::make_index_sequence<std::tuple_size_v
        <std::remove_cvref_t<TupleType>>>());
    return os;
}