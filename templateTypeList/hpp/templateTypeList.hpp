#pragma once
#include <concepts>
#include <cstddef>


// 0) TypeList
// A type list is a compile-time list of types.
template<typename ...Ts>
struct TypeList{};

// Trait for TypeList identification
template<typename T>
struct isTypeList : std::false_type{};

template <typename ...Ts>
struct isTypeList<TypeList<Ts...>> : std::true_type{};

// Concept to check if a type is a TypeList
template<typename T>
concept TypeListConcept = isTypeList<T>::value;

// 1) Function Length
// Returns the number of types in TypeList<Ts...>.
template <TypeListConcept List>
struct Length;

template<typename ...Ts>
struct Length<TypeList<Ts...>>{
    static constexpr std::size_t value = sizeof...(Ts);
};

// Alias for Length
template <TypeListConcept List>
inline constexpr std::size_t Length_t = Length<List>::value;


// 2) Function IndexOf
// Returns the index of the first occurrence of Type in TypeList<Ts...>.
template<TypeListConcept List, typename Type>
struct IndexOf;

//Case: empty list (its tail)
template <typename Type>
struct IndexOf<TypeList<>, Type>{
    public:
    static constexpr int value = -1;
};

// Case: head is target type
template <typename ...Tail, typename Type>
struct IndexOf<TypeList<Type, Tail...>, Type>{
    public:
    static constexpr int value = 0;
};

// Case: head is NOT target type
template <typename Head, typename ...Tail, typename Type>
requires(!std::same_as<Type, Head>)
struct IndexOf<TypeList<Head, Tail...>, Type>{
    private:
    static constexpr int temp = IndexOf<TypeList<Tail...>, Type>::value;
    public:
    static constexpr int value = temp < 0 ? -1 : temp + 1;
};

// Alias for IndexOf
template <TypeListConcept List, typename Type>
inline constexpr int IndexOf_t = IndexOf<List, Type>::value;

// 3) Function PushFront
// Adds a type to the front of TypeList<Ts...>
template <TypeListConcept List, typename Type>
struct PushFront;

template <typename ...Ts, typename Type>
struct PushFront<TypeList<Ts...>, Type>{
    public:
    using type = TypeList<Type, Ts...>;
};

// Alias for PushFront
template <TypeListConcept List, typename Type>
using PushFront_t = typename PushFront<List, Type>::type;

// 4) Function Erase
// Removes the all occurrence of Type from TypeList<Ts...>
template <TypeListConcept List, typename Type>
struct Erase;

// Case: empty list (its tail)
template <typename Type>
struct Erase <TypeList<>, Type>{
    public:
    using type = TypeList<>;
};

// Case: head is target type
template <typename ...Tail, typename Type>
struct Erase<TypeList<Type, Tail...>, Type>{
    private:
    using temp = Erase<TypeList<Tail...>, Type>::type;
    public:
    using type = temp;
};

// Case: head is NOT target type
template <typename Type, typename Head, typename ...Tail>
requires(!std::same_as<Type, Head>)
struct Erase<TypeList<Head, Tail...>, Type>{
    private:
    using temp = Erase<TypeList<Tail...>, Type>::type;
    public:
    using type = PushFront<temp, Head>::type;
};

// Alias for Erase
template <TypeListConcept List, typename Type>
using Erase_t = typename Erase<List, Type>::type;
