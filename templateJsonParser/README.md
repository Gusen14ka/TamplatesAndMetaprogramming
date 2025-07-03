# JSON HasKey Compile-Time Test

Этот файл содержит инструкцию по проверке работы мета‑функции `HasKey`, которая определяет наличие ключа в JSON-подобном `JsonString`.

## Содержимое

1. **JsonString**  
   Пользователь самостоятельно задаёт `JsonString` как:
   ```cpp
   using MyJson = JsonString<'{','"','f','o','o','"',':','1','2','3','}'>;
   ```
   На текущем этапе **автоматическую сериализацию строки в `JsonString<char...>` сделать не удалось**.

2. **Метапрограммная проверка ключа**  
   - Проверка наличия ключа `"foo"`:
     ```cpp
     static_assert(HasKey<MyJson, 'f','o','o'>::value, "foo must exist");
     ```
   - Проверка отсутствия ключа `"bar"`:
     ```cpp
     static_assert(!HasKey<MyJson, 'b','a','r'>::value, "bar must not exist");
     ```

3. **Запуск тестов**  
   Скомпилируйте `templateJsonParser.cpp` с реализацией `HasKey` и `JsonString`, убедитесь, что `static_assert` проходят без ошибок, и проверьте вывод:
   ```bash
   g++ -std=c++20 templateJsonParser.cpp -o test && ./test
   ```

---

> **Примечание**  
> Реализация автоматического преобразования строкового литерала в `JsonString<char...>` на этапе компиляции (через NTTP или UDL) пока оставлена за рамками этого примера.  