#include <type_traits>
namespace my_type_traits {
    // Function to check if type a pointer
    template<typename T>
    struct is_pointer : std::false_type {};

    template<typename T>
    struct is_pointer<T*> : std::true_type {};

    // Type alias to use the is_pointer struct
    template<typename T>
    inline constexpr bool is_pointer_v = is_pointer<T>::value;

    // Function to remove const from a type
    template<typename T>
    struct remove_const{
        using type = T;
    };

    template<typename T>
    struct remove_const<const T>{
        using type = T;
    };
    
    // Type alias to use the remove_const struct
    template<typename T>
    using remove_const_t = remove_const<T>::type;

    // Function to add lvalue reference to a type
    template<typename T>
    struct add_lvalue_reference{
        using type = T&;
    };

    // Specializations for void types
    template<> 
    struct add_lvalue_reference<void> { 
        using type = void; 
    };
    template<> 
    struct add_lvalue_reference<const void> { 
        using type = const void; 
    };
    template<> 
    struct add_lvalue_reference<volatile void> { 
        using type = volatile void; 
    };
    template<>
     struct add_lvalue_reference<const volatile void> { 
        using type = const volatile void; 
    };

    // Type alias to use the add_lvalue_reference struct
    template<typename T>
    using add_lvalue_reference_t = add_lvalue_reference<T>::type;

    // Concept to check if T is an array-like type
    template<typename T>
    concept ArrayLikeC = requires (T t, std::size_t i) {
        t[0];
        t.begin();
        t.end();
    };

    // Function to check if type is an array
    // This includes built-in arrays, std::array, and other array-like types
    template<typename T>
    struct is_array : std::false_type {};

    template<typename T>
    struct is_array<T[]> : std::true_type {};

    template<typename T, std::size_t N>
    struct is_array<T[N]> : std::true_type {};

    /*
    just for exercise add to array array-like types
    template<ArrayLikeC T>
    struct is_array<T> : std::true_type {};
    */
    // Type alias to use the is_array struct
    template<typename T>
    inline constexpr bool is_array_v = is_array<T>::value;

    // Function to check if type is a reference
    // This includes lvalue references and rvalue references
    template<typename T>
    struct is_reference : std::false_type {};

    template<typename T>
    struct is_reference<T&> : std::true_type {};

    template<typename T>
    struct is_reference<T&&> : std::true_type {};

    // Type alias to use the is_reference struct
    template<typename T>
    inline constexpr bool is_reference_v = is_reference<T>::value;

    // Function to check if types are the same
    template<typename T, typename U>
    struct is_same : std::false_type {};

    template<typename T>
    struct is_same<T, T> : std::true_type {};

    template<typename T, typename U>
    inline constexpr bool is_same_v = is_same<T, U>::value;

    // Function to remove volatile
    template<typename T>
    struct remove_volatile {
        using type = T;
    };

    template<typename T>
    struct remove_volatile<volatile T>{
        using type = T;
    };
    
    // Type alias to use remove_volatile struct
    template<typename T>
    using remove_volatile_t = remove_volatile<T>::type;

    //Fuction to remove const and volatile
    template<typename T>
    struct remove_cv{
        using type = remove_volatile_t<remove_const_t<T>>;
    };

    // Type alias to use remove_cv struct
    template<typename T>
    using remove_cv_t = remove_cv<T>::type;

    // Function to remove reference
    template<typename T>
    struct remove_reference{
        using type = T;
    };

    template<typename T>
    struct remove_reference<T&>{
        using type = T;
    };

    template<typename T>
    struct remove_reference<T&&>{
        using type = T;
    };

    // Type alias to use remove_reference
    template<typename T>
    using remove_reference_t = remove_reference<T>::type;

    // Fuction to add pointer
    template<typename T>
    struct add_pointer{
        using type = remove_reference_t<T>*;
    };

    template<typename T>
    using add_pointer_t = add_pointer<T>::type;

    // Fuction to get type array's elemets
    template<typename T>
    struct remove_extent{
        using type = T;
    };

    template<typename T>
    struct remove_extent<T[]>{
        using type = T;
    };

    template<typename T, std::size_t N>
    struct remove_extent<T[N]>{
        using type = T;
    };

    // Type alias to use remove_extent
    template<typename T>
    using remove_extent_t = remove_extent<T>::type;

    // Fuction to check if the type is const
    template<typename T>
    struct is_const : std::false_type {};

    template<typename T>
    struct is_const<const T> : std::true_type {};

    // Type alias to use is_const
    template<typename T>
    inline constexpr bool is_const_v = is_const<T>::value;

    // Fuction to check if type is function
    template<typename T>
    struct is_function {
        static constexpr bool value = (!is_const_v<const T> && !is_reference_v<T>);
    };

    // Type alias to use is_function
    template<typename T>
    inline constexpr bool is_function_v = is_function<T>::value;

    // Concept to check if type is function
    template<typename T>
    concept FunctionLikeC = is_function_v<T>;
    // Fuction decay
    template<typename T>
    struct decay{
        using type = remove_cv_t<remove_reference_t<T>>;
    };
    template<FunctionLikeC T>
    struct decay<T>{
        using type = add_pointer_t<remove_reference_t<T>>;
    };
    template<typename T>
    struct decay<T[]>{
        using type = remove_extent<remove_reference_t<T>>::type*;
    };
    template<typename T, std::size_t N>
    struct decay<T[N]>{
        using type = remove_extent<remove_reference_t<T>>::type*;
    };

    // Type alias to use decay
    template<typename T>
    using decay_t = decay<T>::type;
}