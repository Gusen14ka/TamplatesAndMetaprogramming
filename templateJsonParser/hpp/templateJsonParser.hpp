#pragma once
#include <cstddef>
#include <utility>

// Macros wrapper for creating JSON strings
#define JSON_STRING(str) makeJsonString(str)

template<char... Cs>
struct JsonString {};

// Function to create a JsonString from a string literal
// This function is constexpr and can be used to create a JsonString at compile time.
template<typename CharType, std::size_t N>
consteval auto makeJsonString(const CharType (&str)[N]){
    return [str]<std::size_t ...Ind>(std::index_sequence<Ind...>){
        return JsonString<str[Ind]...>();
    }(std::make_index_sequence<N-1>());
}

// Function to check if Json-Key macthes a given key
template<typename Json, char...Key>
struct MatchKey;

// Case: Json-Key partly matches the given one
template<char Head, char...Tail, char KeyHead, char...KeyTail>
struct MatchKey<JsonString<Head, Tail...>, KeyHead, KeyTail...>{
    static constexpr bool value = (Head == KeyHead) && MatchKey<JsonString<Tail...>, KeyTail...>::value;
};

// Case: given key is empty
template<>
struct MatchKey<JsonString<>>{
    static constexpr bool value = false;
};

// Case: Json-Key completely mached with the given one and after it there are valid characters " and :
template<char... Tail>
struct MatchKey<JsonString<'"', ':', Tail...>>{
    static constexpr bool value = true;
};

// Case: Json-string is empty
template<char ...Key>
struct MatchKey<JsonString<>, Key...>{
    static constexpr bool value = false;
};

// Case: Json-string is a single character
template<char Head>
struct MatchKey<JsonString<Head>>{
    static constexpr bool value = false;
};

// Case: when Key is empty(or ended)
template<char... Tail>
struct MatchKey<JsonString<Tail...>>{
    static constexpr bool value = false;
};

// Fuction to check if a JsonString contains a specific key
template<typename Json, char... Cs>
struct HasKey;

// Case: Json-string is empty
template<char... Key>
struct HasKey<JsonString<>, Key...>{
    static constexpr bool value = false;
};

// Case: comon case - search for " is going on
template<char Head, char...Tail, char...Key>
struct HasKey<JsonString<Head, Tail...>, Key...>{
    static constexpr bool value = HasKey<JsonString<Tail...>, Key...>::value;
};

// Case: the " character is found, now we need to check if the key matches
template<char... Tail, char...Key>
struct HasKey<JsonString<'"', Tail...>, Key...>{
    static constexpr bool value = MatchKey<JsonString<Tail...>, Key...>::value
        | HasKey<JsonString<Tail...>, Key...>::value;
};


