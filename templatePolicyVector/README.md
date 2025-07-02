# Policy-Based Vector (templatePolicyVector.hpp)

**templatePolicyVector.hpp** provides a flexible, header-only `PolicyVector` container whose behavior is customized via template policies:

## Features

- **Allocation Policies**  
  - `NewDelAllocate<T>` — uses global `operator new`/`delete`.  
  - `MallocFreeAllocate<T>` — uses `std::malloc`/`std::free`.

- **Locking Policies**  
  - `NoLock<T>` — no synchronization.  
  - `MutexLock<T>` — uses `std::mutex`.

- **Error Handling Policies**  
  - `NoError<T>` — ignores errors.  
  - `ErrorThrow<T>` — throws `std::out_of_range` on error.

- **RAII LockGuard**  
  - Template accepts any number of lock policies and calls `lock()` in its constructor and `unlock()` in its destructor.

- **Operations**  
  - `push_back(const T&)`, `pop_back()`, `at(size_t)`, `size()`, `capacity()`.  
  - Move constructor and move assignment with proper locking.

## Usage

Include the header and instantiate:

```cpp
#include "templatePolicyVector.hpp"
using namespace policy_vector;

// Create a thread-safe vector of ints:
PolicyVector<int, NewDelAllocate, MutexLock, ErrorThrow> vec;
vec.push_back(1);
int x = vec.at(0);

// Create a simple, non-thread-safe vector that ignores errors:
PolicyVector<double, MallocFreeAllocate, NoLock, NoError> v2;
v2.push_back(3.14);
```

## Testing

A minimal test suite is provided in `templatePolicyVector.cpp`. It covers:

1. **Basic push_back / at / pop_back** with `NoLock` + `NoError`.  
2. **Error policy**: popping from empty, out-of-range access with `ErrorThrow`.  
3. **Geometric growth** up to many elements.  
4. **Move constructor** and **move assignment** semantics.  
5. **Alternative allocators** and **MutexLock**.

Compile and run:

```bash
g++ -std=c++20 -Wall -Wextra templatePolicyVectorR.cpp -o test && ./test
```

You should see:

```
All tests passed.
```

---