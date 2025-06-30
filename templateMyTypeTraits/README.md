# My Type Traits Library

This header-only library implements a set of custom type traits and utilities without relying on the standard `<type_traits>` for core logic. It also provides a `decay` metafunction and concepts for array-like and function-like types.

## Traits Provided

- **Pointer Check**: `is_pointer_v<T>`  
- **Const Removal**: `remove_const_t<T>`  
- **Volatile Removal**: `remove_volatile_t<T>`  
- **CV Removal**: `remove_cv_t<T>`  
- **Lvalue Reference Addition**: `add_lvalue_reference_t<T>`  
- **Reference Check**: `is_reference_v<T>`  
- **Same Type Check**: `is_same_v<T, U>`  
- **Pointer Addition**: `add_pointer_t<T>`  
- **Extent Removal**: `remove_extent_t<T>`  
- **Array Check**: `is_array_v<T>` (covers C-arrays, `std::array`, and `vector`-like containers)  
- **Const Check**: `is_const_v<T>`  
- **Function Check**: `is_function_v<T>` and concept `FunctionLikeC`  
- **Decay**: `decay_t<T>` with correct handling of references, cv-qualifiers, functions, and arrays.

## Usage

Include your header:
```cpp
#include "my_type_traits.hpp"
using namespace my_type_traits;
```

Use the `_v` and `_t` suffix aliases for convenient checks and type transformations:
```cpp
static_assert(is_pointer_v<int*>);
using NoConst = remove_const_t<const int>; // int
using Decayed = decay_t<int[5]>;           // int*
```

## Decay Logic

1. Remove references and cv-qualifiers.
2. If `T` is a function type, decay to function pointer.
3. If `T` is a C-array, decay to pointer to element.
4. Otherwise, keep the base type.