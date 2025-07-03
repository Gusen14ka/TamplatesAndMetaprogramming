#pragma once
#include <cstddef>
#include <utility>

#define JSON_STRING(str) makeJsonString(str)

template<char... Cs>
struct JsonString {};


template<typename CharType, std::size_t N>
consteval auto makeJsonString(const CharType (&str)[N]){
    return [str]<std::size_t ...Ind>(std::index_sequence<Ind...>){
        return JsonString<str[Ind]...>();
    }(std::make_index_sequence<N-1>());
}

template<typename Json, char...Key>
struct MatchKey;

template<char Head, char...Tail, char KeyHead, char...KeyTail>
struct MatchKey<JsonString<Head, Tail...>, KeyHead, KeyTail...>{
    static constexpr bool value = (Head == KeyHead) && MatchKey<JsonString<Tail...>, KeyTail...>::value;
};

template<>
struct MatchKey<JsonString<>>{
    static constexpr bool value = false;
};

template<char... Tail>
struct MatchKey<JsonString<'"', ':', Tail...>>{
    static constexpr bool value = true;
};

template<char ...Key>
struct MatchKey<JsonString<>, Key...>{
    static constexpr bool value = false;
};

template<char Head>
struct MatchKey<JsonString<Head>>{
    static constexpr bool value = false;
};

template<char... Tail>
struct MatchKey<JsonString<Tail...>>{
    static constexpr bool value = false;
};

template<typename Json, char... Cs>
struct HasKey;

template<char... Key>
struct HasKey<JsonString<>, Key...>{
    static constexpr bool value = false;
};

template<char Head, char...Tail, char...Key>
struct HasKey<JsonString<Head, Tail...>, Key...>{
    static constexpr bool value = HasKey<JsonString<Tail...>, Key...>::value;
};

template<char... Tail, char...Key>
struct HasKey<JsonString<'"', Tail...>, Key...>{
    static constexpr bool value = MatchKey<JsonString<Tail...>, Key...>::value
        | HasKey<JsonString<Tail...>, Key...>::value;
};


