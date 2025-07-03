#include <type_traits>
#include <iostream>
#include "../hpp/templateJsonParser.hpp"


int main(){
    // 1) Преобразование строки в JsonString
    using MyJson = JsonString<'{','"', 'f','o','o','"',':','1','2','3','}'>;
    static_assert(std::is_same_v<
      MyJson,
      JsonString<'{','"','f','o','o','"',':','1', '2','3','}'>
    >);

    // 2) Ключ есть
    static_assert(HasKey<MyJson,'f','o','o'>::value,
                  "foo must exist");

    // 3) Ключа нет
    static_assert(!HasKey<MyJson, 'b','a','r'>::value,
                  "bar must not exist");

    std::cout << "All JSON hasKey tests passed\n";
}
