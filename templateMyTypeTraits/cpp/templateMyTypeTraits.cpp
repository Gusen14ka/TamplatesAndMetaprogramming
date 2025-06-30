#include "hpp/templateMuTypeTraits.hpp"
#include <cassert>
#include <vector>
#include <array>
//#include <string>

using namespace my_type_traits;

int main() {
    // is_pointer_v
    static_assert(is_pointer_v<int*>);
    static_assert(!is_pointer_v<int>);
    static_assert(is_pointer_v<void(*)()>);
    
    // remove_const_t
    static_assert(std::is_same_v<remove_const_t<const int>, int>);
    static_assert(std::is_same_v<remove_const_t<int>, int>);
    static_assert(std::is_same_v<remove_const_t<const volatile double>, volatile double>);
    
    // add_lvalue_reference_t
    static_assert(std::is_same_v<add_lvalue_reference_t<int>, int&>);
    static_assert(std::is_same_v<add_lvalue_reference_t<int&>, int&>);
    static_assert(std::is_same_v<add_lvalue_reference_t<void>, void>);
    
    // is_array_v
    static_assert(is_array_v<int[5]>);
    //static_assert(is_array_v<std::array<float,3>>);
    //static_assert(is_array_v<std::vector<double>>);
    static_assert(!is_array_v<int>);
    //static_assert(is_array_v<std::string>);
    
    // is_reference_v
    static_assert(is_reference_v<int&>);
    static_assert(is_reference_v<int&&>);
    static_assert(!is_reference_v<int>);
    
    // is_same_v
    static_assert(is_same_v<int,int>);
    static_assert(!is_same_v<int,long>);
    
    // remove_volatile_t
    static_assert(std::is_same_v<remove_volatile_t<volatile int>, int>);
    static_assert(std::is_same_v<remove_volatile_t<const volatile char>, const char>);
    
    // remove_cv_t
    static_assert(std::is_same_v<remove_cv_t<const volatile int>, int>);
    static_assert(std::is_same_v<remove_cv_t<volatile double>, double>);
    
    // remove_reference_t
    static_assert(std::is_same_v<remove_reference_t<int&>, int>);
    static_assert(std::is_same_v<remove_reference_t<int&&>, int>);
    static_assert(std::is_same_v<remove_reference_t<int>, int>);
    
    // add_pointer_t
    static_assert(std::is_same_v<add_pointer_t<int>, int*>);
    static_assert(std::is_same_v<add_pointer_t<int&>, int*>);
    static_assert(std::is_same_v<add_pointer_t<void>, void*>);
    
    // remove_extent_t
    static_assert(std::is_same_v<remove_extent_t<int[4]>, int>);
    static_assert(std::is_same_v<remove_extent_t<double[]>, double>);
    static_assert(std::is_same_v<remove_extent_t<std::array<char,2>>, std::array<char,2>>);
    
    // is_const_v
    static_assert(is_const_v<const int>);
    static_assert(!is_const_v<int>);
    
    // is_function_v / FunctionLikeC
    static_assert(is_function_v<void()>);
    static_assert(is_function_v<int(double, char)>);
    static_assert(!is_function_v<int*>);
    static_assert(!is_function_v<int&>);
    static_assert(FunctionLikeC<void()>);
    static_assert(!FunctionLikeC<int>);
    
    // decay_t
    // decay lvalue
    static_assert(std::is_same_v<decay_t<int>, int>);
    static_assert(std::is_same_v<decay_t<const int>, int>);
    static_assert(std::is_same_v<decay_t<int&>, int>);
    // decay to pointer for functions
    static_assert(std::is_same_v<decay_t<void()>, void(*)()>);
    // decay arrays
    static_assert(std::is_same_v<decay_t<int[3]>, int*>);
    static_assert(std::is_same_v<decay_t<const char[10]>, const char*>);
    
    // Run-time sanity checks (just to silence unused warnings)
    assert(is_pointer_v<int*>);
    assert(is_array_v<std::vector<int>>);
    assert(FunctionLikeC<void(int)>);
    
    return 0;
}