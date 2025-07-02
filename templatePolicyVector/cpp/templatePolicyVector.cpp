#include <iostream>
#include <cassert>
#include "../hpp/templatePolicyVector.hpp"

using namespace policy_vector;

// Помощный макрос для проверки исключений
#define ASSERT_THROWS(stmt, ex_type)            \
    do {                                        \
        bool thrown = false;                    \
        try { stmt; }                          \
        catch(const ex_type&) { thrown = true; } \
        catch(...) {}                           \
        assert(thrown && "Expected exception"); \
    } while(0)


int main() {
    // 1) Тест базового push_back / at / pop_back (NoLock + NoError)
    {
        PolicyVector<int, NewDelAllocate, NoLock, NoError> v;
        assert(v.size() == 0 && v.capacity() == 0);

        v.push_back(10);
        assert(v.size() == 1);
        assert(v.capacity() >= 1);
        assert(v.at(0) == 10);

        v.push_back(20);
        assert(v.size() == 2);
        assert(v.at(1) == 20);

        v.pop_back();
        assert(v.size() == 1);
        assert(v.at(0) == 10);

        // pop_back пустого — нет обработки (NoError), но код получает UB из-за [[noreturn]]
        // Поэтому не вызываем pop_back() здесь
    }

    // 2) Тест обработки ошибок (NoLock + ErrorThrow)
    {
        PolicyVector<int, NewDelAllocate, NoLock, ErrorThrow> v;
        assert(v.size() == 0);
        ASSERT_THROWS(v.pop_back(), std::out_of_range);
        ASSERT_THROWS(v.at(0),   std::out_of_range);

        v.push_back(1);
        v.push_back(2);
        assert(v.size() == 2);
        ASSERT_THROWS(v.at(5),   std::out_of_range);
    }

    // 3) Тест geometric growth
    {
        PolicyVector<int, NewDelAllocate, NoLock, NoError> v;
        for(int i = 0; i < 1000; ++i) {
            v.push_back(i);
            assert(v.at(i) == i);
        }
        assert(v.size() == 1000);
    }

    // 4) Тест перемещения (move ctor)
    {
        PolicyVector<int, NewDelAllocate, NoLock, NoError> a;
        a.push_back(42);
        a.push_back(99);
        assert(a.size() == 2);

        PolicyVector<int, NewDelAllocate, NoLock, NoError> b(std::move(a));
        // a был «обнулён» в move-ctor
        assert(a.size() == 0);
        // b получил данные
        assert(b.size() == 2);
        assert(b.at(0) == 42);
        assert(b.at(1) == 99);
    }

    // 5) Тест перемещения (move assignment)
    {
        PolicyVector<int, NewDelAllocate, NoLock, NoError> a;
        for(int i = 0; i < 5; ++i) a.push_back(i);

        PolicyVector<int, NewDelAllocate, NoLock, NoError> c;
        c = std::move(a);

        assert(a.size() == 0);
        assert(c.size() == 5);
        for(int i = 0; i < 5; ++i) assert(c.at(i) == i);
    }

    // 6) Тест разных аллокаторов и потокобезопасность (MutexLock)
    {
        PolicyVector<int, MallocFreeAllocate, MutexLock, NoError> v;
        for(int i = 0; i < 10; ++i) v.push_back(i*2);
        for(int i = 0; i < 10; ++i) assert(v.at(i) == i*2);
    }

    std::cout << "All tests passed.\n";
    return 0;
}
